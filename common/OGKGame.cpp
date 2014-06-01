//
//  OGKGame.cpp
//  OgreGameKit
//
//  Created by Alex on 5/20/14.
//
//

#include "OGKGame.h"

#include "macUtils.h"

using namespace Ogre;

namespace Ogre
{
    template<> OGKGame* Ogre::Singleton<OGKGame>::msSingleton = 0;
};

OGKGame::OGKGame()
{
	m_MoveSpeed			= 0.1f;
	m_RotateSpeed       = 0.3f;
    m_StartTime         = 0;
    m_TimeSinceLastFrame = 0;
	m_bShutDownOgre     = false;
	m_iNumScreenShots   = 0;
    
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

//|||||||||||||||||||||||||||||||||||||||||||||||
#if defined(OGRE_IS_IOS)
bool OGKGame::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MultiTouchListener *pMouseListener)
#else
bool OGKGame::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
#endif
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();
    
    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);
    
    String pluginsPath;
    // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
    pluginsPath = m_ResourcePath + "plugins.cfg";
#endif
    
    m_pRoot = new Ogre::Root(pluginsPath, Ogre::macBundlePath() + "/ogre.cfg");
    
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.load();
#endif
    
	if(!m_pRoot->showConfigDialog())
		return false;
	m_pRenderWnd = m_pRoot->initialise(true, wndTitle);
    
	m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
    
    m_pOverlaySystem = new Ogre::OverlaySystem();
    m_pSceneMgr->addRenderQueueListener(m_pOverlaySystem);
    
	m_pCamera = m_pSceneMgr->createCamera("Camera");
	m_pCamera->setPosition(Vector3(0, 60, 60));
	m_pCamera->lookAt(Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(1);
    
	m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
	m_pViewport->setBackgroundColour(ColourValue(0.8f, 0.7f, 0.6f, 1.0f));
    
	m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));
	
	m_pViewport->setCamera(m_pCamera);
    
	unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);
    
	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
    
	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);
    
#if !defined(OGRE_IS_IOS)
    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));
    
	m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
	m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();
#else
	m_pMouse = static_cast<OIS::MultiTouch*>(m_pInputMgr->createInputObject(OIS::OISMultiTouch, true));
#endif
    
#if !defined(OGRE_IS_IOS)
	if(pKeyListener == 0)
		m_pKeyboard->setEventCallback(this);
	else
		m_pKeyboard->setEventCallback(pKeyListener);
#endif
    
	if(pMouseListener == 0)
		m_pMouse->setEventCallback(this);
	else
		m_pMouse->setEventCallback(pMouseListener);
    
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
    
	m_pTimer = OGRE_NEW Ogre::Timer();
	m_pTimer->reset();
    
	m_pRenderWnd->setActive(true);
    
	return true;
}

OGKGame::~OGKGame()
{
    if(m_pInputMgr) OIS::InputManager::destroyInputSystem(m_pInputMgr);
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.unload();
#endif
    if(m_pRoot) delete m_pRoot;
    
#ifdef INCLUDE_RTSHADER_SYSTEM
    mShaderGenerator->removeSceneManager(OGKGame::getSingletonPtr()->m_pSceneMgr);
    
    destroyRTShaderSystem();
#endif
}

#ifdef INCLUDE_RTSHADER_SYSTEM

/*-----------------------------------------------------------------------------
 | Initialize the RT Shader system.
 -----------------------------------------------------------------------------*/
bool OGKGame::initialiseRTShaderSystem(Ogre::SceneManager* sceneMgr)
{
    if (Ogre::RTShader::ShaderGenerator::initialize())
    {
        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
        
        mShaderGenerator->addSceneManager(sceneMgr);
        
        // Setup core libraries and shader cache path.
        Ogre::StringVector groupVector = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
        Ogre::StringVector::iterator itGroup = groupVector.begin();
        Ogre::StringVector::iterator itGroupEnd = groupVector.end();
        Ogre::String shaderCoreLibsPath;
        Ogre::String shaderCachePath;
        
        for (; itGroup != itGroupEnd; ++itGroup)
        {
            Ogre::ResourceGroupManager::LocationList resLocationsList = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(*itGroup);
            Ogre::ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
            Ogre::ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
            bool coreLibsFound = false;
            
            // Try to find the location of the core shader lib functions and use it
            // as shader cache path as well - this will reduce the number of generated files
            // when running from different directories.
            for (; it != itEnd; ++it)
            {
                if ((*it)->archive->getName().find("RTShaderLib") != Ogre::String::npos)
                {
                    shaderCoreLibsPath = (*it)->archive->getName() + "/";
                    shaderCachePath = shaderCoreLibsPath;
                    coreLibsFound = true;
                    break;
                }
            }
            // Core libs path found in the current group.
            if (coreLibsFound)
                break;
        }
        
        // Core shader libs not found -> shader generating will fail.
        if (shaderCoreLibsPath.empty())
            return false;
        
        // Create and register the material manager listener.
        mMaterialMgrListener = new OGKShaderGeneratorListener(mShaderGenerator);
        Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
    }
    
    Ogre::MaterialPtr baseWhite = Ogre::MaterialManager::getSingleton().getByName("BaseWhite", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
    baseWhite->setLightingEnabled(false);
    mShaderGenerator->createShaderBasedTechnique(
                                                 "BaseWhite",
                                                 Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
                                                 Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
    mShaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                                       "BaseWhite");
    baseWhite->getTechnique(0)->getPass(0)->setVertexProgram(
                                                             baseWhite->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
    baseWhite->getTechnique(0)->getPass(0)->setFragmentProgram(
                                                               baseWhite->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
    
    // creates shaders for base material BaseWhiteNoLighting using the RTSS
    mShaderGenerator->createShaderBasedTechnique(
                                                 "BaseWhiteNoLighting",
                                                 Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
                                                 Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
    mShaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                                       "BaseWhiteNoLighting");
    Ogre::MaterialPtr baseWhiteNoLighting = Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
    baseWhiteNoLighting->getTechnique(0)->getPass(0)->setVertexProgram(
                                                                       baseWhiteNoLighting->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
    baseWhiteNoLighting->getTechnique(0)->getPass(0)->setFragmentProgram(
                                                                         baseWhiteNoLighting->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
    return true;
}

/*-----------------------------------------------------------------------------
 | Destroy the RT Shader system.
 -----------------------------------------------------------------------------*/
void OGKGame::destroyRTShaderSystem()
{
    // Restore default scheme.
    Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);
    
    // Unregister the material manager listener.
    if (mMaterialMgrListener != NULL)
    {
        Ogre::MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
        delete mMaterialMgrListener;
        mMaterialMgrListener = NULL;
    }
    
    // Destroy RTShader system.
    if (mShaderGenerator != NULL)
    {
        Ogre::RTShader::ShaderGenerator::destroy();
        mShaderGenerator = NULL;
    }
}
#endif // INCLUDE_RTSHADER_SYSTEM

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
		m_pRenderWnd->writeContentsToTimestampedFile("BOF_Screenshot_", ".png");
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

bool OGKGame::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

#if defined(OGRE_IS_IOS)
bool OGKGame::touchMoved(const OIS::MultiTouchEvent &evt)
{
    OIS::MultiTouchState state = evt.state;
    int origTransX = 0, origTransY = 0;
#if !OGRE_NO_VIEWPORT_ORIENTATIONMODE
    switch(m_pCamera->getViewport()->getOrientationMode())
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
#endif
	m_pCamera->yaw(Degree(state.X.rel * -0.1));
	m_pCamera->pitch(Degree(state.Y.rel * -0.1));
	
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OGKGame::touchPressed(const OIS:: MultiTouchEvent &evt)
{
#pragma unused(evt)
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OGKGame::touchReleased(const OIS:: MultiTouchEvent &evt)
{
#pragma unused(evt)
	return true;
}

bool OGKGame::touchCancelled(const OIS:: MultiTouchEvent &evt)
{
#pragma unused(evt)
	return true;
}
#else
bool OGKGame::mouseMoved(const OIS::MouseEvent &evt)
{
	m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
	m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
	
	return true;
}

bool OGKGame::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

bool OGKGame::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}
#endif

bool OGKGame::renderOneFrame()
{
    if(OGKGame::getSingletonPtr()->isOgreToBeShutDown() ||
       !Ogre::Root::getSingletonPtr() ||
       !Ogre::Root::getSingleton().isInitialised()) {
           return false;
    }
       
    if(m_pRenderWnd->isActive()) {
        m_StartTime = m_pTimer->getMillisecondsCPU();
        
        m_pMouse->capture();
        update(m_TimeSinceLastFrame);
        m_pRoot->renderOneFrame();
        
        m_TimeSinceLastFrame = m_pTimer->getMillisecondsCPU() - m_StartTime;
    }
    
    return true;
}

void OGKGame::setup()
{
	m_pSceneMgr->setSkyBox(true, "OGK/DefaultSkyBox");
	m_pSceneMgr->createLight("Light")->setPosition(75,75,75);
}

void OGKGame::start()
{
    if(!initOgre("APP Name", this, 0)) {
        return;
    }
    
    m_bShutDownOgre = false;
    
	m_pLog->logMessage("Game initialized!");
    
#ifdef INCLUDE_RTSHADER_SYSTEM
    initialiseRTShaderSystem(m_pSceneMgr);
#endif
    
	setup();
#if !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
    m_pLog->logMessage("Start main loop...");
	
	double timeSinceLastFrame = 0;
	double startTime = 0;
    
    m_pRenderWnd->resetStatistics();
    
#if (!defined(OGRE_IS_IOS)) && !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
	while(!m_bShutdown && !isOgreToBeShutDown())
	{
		if(m_pRenderWnd->isClosed())m_bShutdown = true;
        
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		Ogre::WindowEventUtilities::messagePump();
#endif
		if(m_pRenderWnd->isActive())
		{
			startTime = m_pTimer->getMillisecondsCPU();
            
#if !OGRE_IS_IOS
			m_pKeyboard->capture();
#endif
			m_pMouse->capture();
			updateOgre(timeSinceLastFrame);
			m_pRoot->renderOneFrame();
            
			timeSinceLastFrame = m_pTimer->getMillisecondsCPU() - startTime;
		}
		else
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
		}
	}
#endif
    
#if !defined(OGRE_IS_IOS)
	m_pLog->logMessage("Main loop quit");
	m_pLog->logMessage("Shutdown OGRE...");
#endif
#endif
    
}

void OGKGame::update(double timeSinceLastFrame)
{
	m_MoveScale = m_MoveSpeed   * (float)timeSinceLastFrame;
	m_RotScale  = m_RotateSpeed * (float)timeSinceLastFrame;
    
    m_pSceneMgr->setSkyBoxEnabled(true);
    
	m_TranslateVector = Vector3::ZERO;
    
	getInput();
	moveCamera();
}

void OGKGame::moveCamera()
{
#if !defined(OGRE_IS_IOS)
	if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
		m_pCamera->moveRelative(m_TranslateVector);
	else
#endif
        
		m_pCamera->moveRelative(m_TranslateVector / 10);
}

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
