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
	m_MoveSpeed			= 10.0f;
	m_RotateSpeed       = 0.3f;
    m_StartTime         = 0;
    m_TimeSinceLastFrame = 0;
	m_bShutDownOgre     = false;
    
	m_pRoot				= 0;
	m_pSceneMgr			= 0;
	m_pRenderWnd        = 0;
	m_pCamera			= 0;
	m_pViewport			= 0;
	m_pLog				= 0;
	m_pTimer			= 0;
    
	m_pInputMgr			= 0;
	m_pKeyboard			= 0;
	m_pMouse			= 0;
    
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
    if(m_pInputMgr) OIS::InputManager::destroyInputSystem(m_pInputMgr);
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.unload();
#endif
    if(m_pRoot) delete m_pRoot;
    
#ifdef INCLUDE_RTSHADER_SYSTEM
    destroyRTShaderSystem(m_pSceneMgr);
#endif
}


////////////////////////////////////////////////////////////////////////////////
bool OGKGame::initOgre(Ogre::String wndTitle)
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();
    
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
    
	m_pCamera = m_pSceneMgr->createCamera("Camera");
	m_pCamera->setPosition(Vector3(0, 1000, 60));
	m_pCamera->lookAt(Vector3(0, 1000, 0));
	m_pCamera->setNearClipDistance(1);
//    m_pCamera->setPolygonMode(Ogre::PM_WIREFRAME);
    m_pCamera->setFarClipDistance(11000);
    
	m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
	m_pViewport->setBackgroundColour(ColourValue(0.8f, 0.7f, 0.6f, 1.0f));
    
	m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));
	
	m_pViewport->setCamera(m_pCamera);
    
    // create overlay system BEFORE initializing resources (for fonts)
    m_pOverlaySystem = new Ogre::OverlaySystem();
    m_pSceneMgr->addRenderQueueListener(m_pOverlaySystem);
    
	// INPUT
    _initInput();
    
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
void OGKGame::moveCamera()
{
#if !defined(OGRE_IS_IOS)
	if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
		m_pCamera->moveRelative(m_TranslateVector);
	else
#endif
        
    m_pCamera->moveRelative(m_TranslateVector / 10.f);
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
        
#if !defined(OGRE_IS_IOS)
        m_pKeyboard->capture();
#endif
        m_pMouse->capture();
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
//    Ogre::ColourValue fogColour(189.0/255.0, 227.0/255.0, 255.0/255.0);
    Ogre::ColourValue fogColour(184.0/255.0, 223.0/255.0, 251.0/255.0);
    m_pSceneMgr->setFog(Ogre::FOG_LINEAR, fogColour, 0.0, 1000, 4000);
    m_pRenderWnd->getViewport(0)->setBackgroundColour(fogColour);
    
    mTerrain = OGRE_NEW OGKTerrain();
    mTerrain->setup(m_pSceneMgr, light);
    
    playBackgroundMusic("media/audio/background.mp3");
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::start()
{
    if(!initOgre("Ogre Game Kit")) {
        m_pLog->logMessage("Failed to init Ogre");
        return;
    }
    
    m_bShutDownOgre = false;
    
	m_pLog->logMessage("Ogre initialized!");
    
    m_StartTime = m_pTimer->getMillisecondsCPU();
    
#ifdef INCLUDE_RTSHADER_SYSTEM
    initShaderGenerator(m_pSceneMgr);
#endif
    
	setup();

    m_pRenderWnd->resetStatistics();

#if !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__) && !defined(OGRE_IS_IOS)
    m_pLog->logMessage("Start main loop...");
	
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
	m_MoveScale = m_MoveSpeed   * (float)timeSinceLastFrame;
	m_RotScale  = m_RotateSpeed * (float)timeSinceLastFrame;
    
	m_TranslateVector = Vector3::ZERO;
    
	getInput();
	moveCamera();
    
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
void OGKGame::getInput()
{
#if !defined(OGRE_IS_IOS)
	if(m_pKeyboard->isKeyDown(OIS::KC_A))
		m_TranslateVector.x = -m_MoveScale;
	
	if(m_pKeyboard->isKeyDown(OIS::KC_D))
		m_TranslateVector.x = m_MoveScale;
	
	if(m_pKeyboard->isKeyDown(OIS::KC_W))
		m_TranslateVector.z = -m_MoveScale;
	
	if(m_pKeyboard->isKeyDown(OIS::KC_S))
		m_TranslateVector.z = m_MoveScale;
#endif
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::keyPressed(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
	if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
        m_bShutDownOgre = true;
        return true;
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
	{
		m_pRenderWnd->writeContentsToTimestampedFile("OGK_Screenshot_", ".png");
		return true;
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_M))
	{
		static int mode = 0;
		
		if(mode == 2)
		{
			m_pCamera->setPolygonMode(PM_SOLID);
			mode = 0;
		}
		else if(mode == 0)
		{
            m_pCamera->setPolygonMode(PM_WIREFRAME);
            mode = 1;
		}
		else if(mode == 1)
		{
			m_pCamera->setPolygonMode(PM_POINTS);
			mode = 2;
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
    OIS::MultiTouchState state = evt.state;
    int origTransX = 0, origTransY = 0;
#if !OGRE_NO_VIEWPORT_ORIENTATIONMODE
    switch(m_pCamera->getViewport()->getOrientationMode())
#else
    switch(mViewportOrientation)
#endif
    {
        case Ogre::OR_LANDSCAPELEFT:
            origTransX = state.X.rel;
            origTransY = state.Y.rel;
            state.X.rel = -origTransY;
            state.Y.rel = origTransX;
            break;
            
        case Ogre::OR_LANDSCAPERIGHT:
            origTransX = state.X.rel;
            origTransY = state.Y.rel;
            state.X.rel = origTransY;
            state.Y.rel = origTransX;
            break;
            
            // Portrait doesn't need any change
        case Ogre::OR_PORTRAIT:
        default:
            break;
    }

	m_pCamera->yaw(Degree(state.X.rel * -0.1));
	m_pCamera->pitch(Degree(state.Y.rel * -0.1));
	
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
	m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
	m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
	
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
    unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);
    
	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
    
	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);
    
#if !defined(OGRE_IS_IOS)
    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pKeyboard->setEventCallback(this);
    
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));
    
	m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
	m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();
#else
	m_pMouse = static_cast<OIS::MultiTouch*>(m_pInputMgr->createInputObject(OIS::OISMultiTouch, true));
#endif
    
    m_pMouse->setEventCallback(this);
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
