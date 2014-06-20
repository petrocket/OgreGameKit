//
//  OGKGame.cpp
//  OgreGameKit
//
//  Created by Alex on 5/20/14.
//
//

#include "OGKGame.h"

#include "macUtils.h"

#ifdef INCLUDE_RTSHADER_SYSTEM
#include "OGKShaderGenerator.h"
#endif

using namespace Ogre;

namespace Ogre
{
    template<> OGKGame* Ogre::Singleton<OGKGame>::msSingleton = 0;
};

////////////////////////////////////////////////////////////////////////////////
OGKGame::OGKGame() :
#ifdef OGRE_IS_IOS
    mViewportOrientation(Ogre::OR_LANDSCAPELEFT),
#endif
    mTerrain(NULL)
{
	m_MoveSpeed			= 1.0f;
	m_RotateSpeed       = 0.3f;
    m_StartTime         = 0;
    m_TimeSinceLastFrame = 0;
	m_bShutDownOgre     = false;
    
	m_pRoot				= 0;
	m_pSceneMgr			= 0;
	m_pRenderWnd        = 0;
	mCamera			= 0;
	m_pLog				= 0;
	m_pTimer			= 0;
    
    mPlayer             = 0;
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = macBundlePath() + "/Contents/Resources/";
#elif defined(OGRE_IS_IOS)
    m_ResourcePath = macBundlePath() + "/";
#else
    m_ResourcePath = "";
#endif
}

////////////////////////////////////////////////////////////////////////////////
OGKGame::~OGKGame()
{
    delete OGKInputManager::getSingletonPtr();
    
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.unload();
#endif
    if(m_pRoot) delete m_pRoot;
    
#ifdef INCLUDE_RTSHADER_SYSTEM
    destroyRTShaderSystem(m_pSceneMgr);
#endif
}

////////////////////////////////////////////////////////////////////////////////
OGKTerrain *OGKGame::getTerrain()
{
    return mTerrain;
}


////////////////////////////////////////////////////////////////////////////////
bool OGKGame::initOgre(Ogre::String wndTitle)
{
    new Ogre::LogManager();
    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log",
                                                        true, true, false);
    m_pLog->setDebugOutputEnabled(true);
    
    String pluginsPath;
    // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
    pluginsPath = m_ResourcePath + "plugins.cfg";
#endif
    
    m_pRoot = new Ogre::Root(pluginsPath, m_ResourcePath + "ogre.cfg");
    
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.load();
#endif
    
	if(!m_pRoot->showConfigDialog())
		return false;
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_pRoot->getRenderSystem()->setConfigOption("macAPI","cocoa");
#endif
	m_pRenderWnd = m_pRoot->initialise(true, wndTitle);
    
	m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.1f, 0.12f, 0.3f));
    
	// INPUT
    _initInput();
    
    // CAMERA (after input)
    mCamera = OGRE_NEW OGKCamera(m_pSceneMgr, m_pRenderWnd);
//    mCamera->getCamera()->setPosition(0, 1000, 60);
//    mCamera->getCamera()->lookAt(Vector3(0, 1000, 0));
    
    // create overlay system BEFORE initializing resources (for fonts)
    m_pOverlaySystem = new Ogre::OverlaySystem();
    m_pSceneMgr->addRenderQueueListener(m_pOverlaySystem);
    
    // RESOURCES
    _initResources();
    
    // OVERLAYS
    _initOverlays();
    
	m_pTimer = OGRE_NEW Ogre::Timer();
	m_pTimer->reset();
    
	m_pRenderWnd->setActive(true);
    
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::renderOneFrame()
{
    // NOTE: OSX uses NSDate via it's OGKAppDelegate instead of this
    // because the Ogre timer doesn't seem to report correct time always
    double current_time = m_pTimer->getMillisecondsCPU();
    m_TimeSinceLastFrame = current_time - m_StartTime;
    m_StartTime = current_time;
    
    return renderOneFrame(m_TimeSinceLastFrame);
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::renderOneFrame(double timeSinceLastFrame)
{
    if(OGKGame::getSingletonPtr()->isOgreToBeShutDown() ||
       !Ogre::Root::getSingletonPtr() ||
       !Ogre::Root::getSingleton().isInitialised()) {
        return false;
    }
    
    if(m_pRenderWnd->isActive()) {
        
        // input
        OGKInputManager::getSingletonPtr()->capture();
        
        update(timeSinceLastFrame);
        
        m_pRoot->renderOneFrame(timeSinceLastFrame);
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::setup()
{
	m_pSceneMgr->setSkyBox(true, "OGK/DefaultSkyBox");

    Ogre::Vector3 lightDir(0.55,-0.3,0.75);
    lightDir.normalise();
    Ogre::Light *light = m_pSceneMgr->createLight("Light");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightDir);
    light->setDiffuseColour(Ogre::ColourValue::White);

    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);
    
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.1,0.15,0.4));
    Ogre::ColourValue fogColour(184.0/255.0, 223.0/255.0, 251.0/255.0);
    m_pSceneMgr->setFog(Ogre::FOG_LINEAR, fogColour, 0.0, 1000, 4000);
    m_pRenderWnd->getViewport(0)->setBackgroundColour(fogColour);
    
    mTerrain = OGRE_NEW OGKTerrain();
    mTerrain->setup(m_pSceneMgr, light);
    
    playBackgroundMusic("media/audio/background.mp3");
    
    mPlayer = OGRE_NEW OGKPlayer(m_pSceneMgr);
    mPlayer->setEnabled(true);
    
    mCamera->setTarget(mPlayer->getSceneNode());    
    mCamera->setMode(OGKCamera::THIRD_PERSON);
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::start()
{
    if(!initOgre("Ogre Game Kit")) {
        m_pLog->logMessage("Failed to init Ogre", Ogre::LML_CRITICAL);
        return;
    }
    
    m_bShutDownOgre = false;
    
	m_pLog->logMessage("Ogre initialized!", Ogre::LML_TRIVIAL);
    
    m_StartTime = m_pTimer->getMillisecondsCPU();
    
#ifdef INCLUDE_RTSHADER_SYSTEM
    initShaderGenerator(m_pSceneMgr);
#endif
    
	setup();

    m_pRenderWnd->resetStatistics();

#if !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__) && !defined(OGRE_IS_IOS)
    m_pLog->logMessage("Start main loop...", Ogre::LML_TRIVIAL);
	
	double timeSinceLastFrame = 0;
	double startTime = 0;
    
	while(!m_bShutdown && !isOgreToBeShutDown())
	{
		if(m_pRenderWnd->isClosed())m_bShutdown = true;
        
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		Ogre::WindowEventUtilities::messagePump();
#endif
		if(m_pRenderWnd->isActive()) {
			startTime = m_pTimer->getMillisecondsCPU();
            
			m_pKeyboard->capture();
			m_pMouse->capture();
			updateOgre(timeSinceLastFrame);
			m_pRoot->renderOneFrame();
            
			timeSinceLastFrame = m_pTimer->getMillisecondsCPU() - startTime;
		}
		else {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
		}
	}
    
	m_pLog->logMessage("Main loop quit");
	m_pLog->logMessage("Shutdown OGRE...");
#endif
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::update(double timeSinceLastFrame)
{
    mPlayer->update(timeSinceLastFrame);

    mCamera->update(timeSinceLastFrame);

    if(m_pOverlay->isVisible()) {
        int fps = (int)floorf(m_pRenderWnd->getLastFPS());
        int ms = (int)ceil(timeSinceLastFrame);
        m_pFPS->setCaption(Ogre::StringConverter::toString(fps) + "fps " + Ogre::StringConverter::toString(ms) + "ms");
    }
    
    if(mTerrain) {
        mTerrain->update();
    }
}

#pragma mark - Input

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::keyPressed(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
    OIS::Keyboard *keyboard = OGKInputManager::getSingletonPtr()->getKeyboard();
    
	if(keyboard->isKeyDown(OIS::KC_ESCAPE))
	{
        m_bShutDownOgre = true;
        return true;
	}
    
	if(keyboard->isKeyDown(OIS::KC_SYSRQ))
	{
		m_pRenderWnd->writeContentsToTimestampedFile("OGK_Screenshot_", ".png");
		return true;
	}
    
	if(keyboard->isKeyDown(OIS::KC_M))
	{
		static int mode = 0;
		
		if(mode == 2)
		{
			mCamera->getCamera()->setPolygonMode(PM_SOLID);
			mode = 0;
		}
		else if(mode == 0)
		{
            mCamera->getCamera()->setPolygonMode(PM_WIREFRAME);
            mode = 1;
		}
		else if(mode == 1)
		{
			mCamera->getCamera()->setPolygonMode(PM_POINTS);
			mode = 2;
		}
	}
    
    if(keyEventRef.key == OIS::KC_C) {
        if(mCamera->getMode() == OGKCamera::FREE) {
            mCamera->setMode(OGKCamera::THIRD_PERSON);
            mPlayer->setEnabled(true);
            
            // show the mouse
            OGKInputManager::getSingletonPtr()->setMouseVisible(true);
        }
        else {
            mCamera->setMode(OGKCamera::FREE);
            mPlayer->setEnabled(false);
            OGKInputManager::getSingletonPtr()->setMouseVisible(false);
        }
    }
#endif
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	return true;
}

#if defined(OGRE_IS_IOS)
////////////////////////////////////////////////////////////////////////////////
bool OGKGame::touchMoved(const OIS::MultiTouchEvent &evt)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::touchPressed(const OIS:: MultiTouchEvent &evt)
{
#pragma unused(evt)
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::touchReleased(const OIS:: MultiTouchEvent &evt)
{
#pragma unused(evt)
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::touchCancelled(const OIS:: MultiTouchEvent &evt)
{
#pragma unused(evt)
	return true;
}
#else
////////////////////////////////////////////////////////////////////////////////
bool OGKGame::mouseMoved(const OIS::MouseEvent &evt)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}
#endif

#pragma mark - Private

////////////////////////////////////////////////////////////////////////////////
void OGKGame::_initInput()
{
    OGKInputManager::getSingletonPtr()->initialise(m_pRenderWnd);
    
    OGKInputManager::getSingletonPtr()->addKeyListener(this, "OGKGameListener");
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->addMultiTouchListener(this, "OGKGameListener");
#else
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "OGKGameListener");
#endif
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::_initResources()
{
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
    cf.load(m_ResourcePath + "resources.cfg");
    
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || defined(OGRE_IS_IOS)
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative dirs
                archName = Ogre::String(m_ResourcePath + archName);
#endif
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::_initOverlays()
{    
    // Main Overlay
    m_pOverlay = Ogre::OverlayManager::getSingleton().create("MainOverlay");
    
    // Container Panel
    m_pOverlayContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "container1"));
    m_pOverlayContainer->setDimensions(0.3f,0.3f);
    m_pOverlayContainer->setPosition(0,0);
    m_pOverlay->add2D(m_pOverlayContainer);
    
    // FPS Text
    m_pFPS = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "id1");
    m_pFPS->setDimensions(0.2f,0.2f);
    m_pFPS->setMetricsMode(Ogre::GMM_PIXELS);
    m_pFPS->setPosition(10,10);
    m_pFPS->setParameter("font_name","SdkTrays/Caption");
    m_pFPS->setParameter("char_height", "15");
    m_pFPS->setCaption("hello");
    m_pFPS->setColour(Ogre::ColourValue(1.0, 1.0, 1.0));
    m_pOverlayContainer->addChild(m_pFPS);
    
    m_pOverlay->show();
}
