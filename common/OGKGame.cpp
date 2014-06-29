//
//  OGKGame.cpp
//  OgreGameKit
//
//  Created by Alex on 5/20/14.
//
//

#include "OGKGame.h"
#include "OGKConsole.h"
#include "macUtils.h"

// scenes
#include "OGKMenuScene.h"
#include "OGKInGameScene.h"

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
    mCamera(NULL),
    mConfig(NULL),
    mFPS(NULL),
    mLog(NULL),
    mPlayer(NULL),
    mRenderWindow(NULL),
    mRoot(NULL),
    mSceneManager(NULL),
    mShutdown(FALSE),
    mStartTime(0),
    mTerrain(NULL),
    mTimer(NULL),
    mTimeSinceLastFrame(0)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    mResourcePath = macBundlePath() + "/Contents/Resources/";
#elif defined(OGRE_IS_IOS)
    mResourcePath = macBundlePath() + "/";
#else
    mResourcePath = "";
#endif
}

////////////////////////////////////////////////////////////////////////////////
OGKGame::~OGKGame()
{
    delete OGKInputManager::getSingletonPtr();
    
#ifdef OGRE_STATIC_LIB
    mStaticPluginLoader.unload();
#endif
    if(mRoot) delete mRoot;
    
#ifdef INCLUDE_RTSHADER_SYSTEM
    destroyRTShaderSystem(mSceneManager);
#endif
}

////////////////////////////////////////////////////////////////////////////////
Ogre::ConfigFile* OGKGame::getGameConfig()
{
    return mConfig;
}

////////////////////////////////////////////////////////////////////////////////
OGKTerrain *OGKGame::getTerrain()
{
    return mTerrain;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::renderOneFrame()
{
    // NOTE: OSX uses NSDate via it's OGKAppDelegate instead of this
    // because the Ogre timer doesn't seem to report correct time always
    double current_time = mTimer->getMillisecondsCPU();
    mTimeSinceLastFrame = current_time - mStartTime;
    mStartTime = current_time;
    
    return renderOneFrame(mTimeSinceLastFrame);
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::renderOneFrame(double timeSinceLastFrame)
{
    if(mShutdown || !Ogre::Root::getSingletonPtr() ||
       !Ogre::Root::getSingleton().isInitialised()) {
        return false;
    }
    
    if(mRenderWindow->isActive()) {
        OGKInputManager::getSingletonPtr()->capture();
        
        update(timeSinceLastFrame);
        
        mRoot->renderOneFrame(timeSinceLastFrame);
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::setup()
{
    // @TODO Load from .scene file instead?
    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

//    _initRTTTest();

    OGKMenuScene *firstScene = OGRE_NEW OGKMenuScene("menu");
    mGameSceneManager->addScene(firstScene,"menu");
    
    OGKInGameScene *secondScene = OGRE_NEW OGKInGameScene("ingame");
    mGameSceneManager->addScene(secondScene,"ingame");
    
    mGameSceneManager->setActiveScene("menu");
    
    
    /*
	mSceneManager->setSkyBox(true, "OGK/DefaultSkyBox");

    Ogre::Vector3 lightDir(0.55,-0.3,0.75);
    lightDir.normalise();
    Ogre::Light *light = mSceneManager->createLight("Light");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightDir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.1,0.15,0.4));
    Ogre::ColourValue fogColour(184.0/255.0, 223.0/255.0, 251.0/255.0);
    mSceneManager->setFog(Ogre::FOG_LINEAR, fogColour, 0.0, 1000, 4000);
    mRenderWindow->getViewport(0)->setBackgroundColour(fogColour);
    
    mTerrain = OGRE_NEW OGKTerrain();
    mTerrain->setup(mSceneManager, light);
    
    playBackgroundMusic("media/audio/background.mp3");
    
    mPlayer = OGRE_NEW OGKPlayer(mSceneManager);
    mPlayer->setEnabled(true);
    
    mCamera->setTarget(mPlayer->getSceneNode());
    mCamera->loadFromConfig();
     */
//    if(mCamera->getMode() == OGKCamera::THIRD_PERSON_INDIRECT) {
//        OGKInputManager::getSingletonPtr()->setMouseVisible(true);
//    }
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::start()
{
    if(!_init("Ogre Game Kit")) {
        mLog->logMessage("Failed to init Ogre", Ogre::LML_CRITICAL);
        return;
    }

    mShutdown = false;
    
	mLog->logMessage("Ogre initialized!");
    
    mStartTime = mTimer->getMillisecondsCPU();
    
#ifdef INCLUDE_RTSHADER_SYSTEM
    initShaderGenerator(mSceneManager);
#endif
    
	setup();

    mRenderWindow->resetStatistics();

#if !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__) && !defined(OGRE_IS_IOS)
    mLog->logMessage("Start main loop...", Ogre::LML_TRIVIAL);
	
	double timeSinceLastFrame = 0;
	double startTime = 0;
    
	while(!mShutdown && !isOgreToBeShutDown())
	{
		if(mRenderWindow->isClosed())mShutdown = true;
        
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		Ogre::WindowEventUtilities::messagePump();
#endif
		if(mRenderWindow->isActive()) {
			startTime = mTimer->getMillisecondsCPU();
            
			updateOgre(timeSinceLastFrame);
			mRoot->renderOneFrame();
            
			timeSinceLastFrame = mTimer->getMillisecondsCPU() - startTime;
		}
		else {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
		}
	}
    
	mLog->logMessage("Main loop quit");
	mLog->logMessage("Shutdown OGRE...");
#endif
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::update(double timeSinceLastFrame)
{
    //mPlayer->update(timeSinceLastFrame);

    //mCamera->update(timeSinceLastFrame);

    mGameSceneManager->update(timeSinceLastFrame);
    
    if(mFPS) {
        std::stringstream s;
        s << "FPS: " <<
        mRenderWindow->getLastFPS() <<
        ", Batches: " <<
        Ogre::Root::getSingletonPtr()->getRenderSystem()->_getBatchCount() << "\n";
        mFPS->text(s.str());
    }

    if(mTerrain) mTerrain->update();
}

#pragma mark - Input

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::keyPressed(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
    if(OGKConsole::getSingletonPtr() && OGKConsole::getSingletonPtr()->isVisible()) {
        OGKConsole::getSingletonPtr()->onKeyPressed(keyEventRef);
        if(keyEventRef.key == OIS::KC_GRAVE) {
            OGKConsole::getSingletonPtr()->setVisible(false);
        }
        return true;
    }
    
    switch (keyEventRef.key) {
        case OIS::KC_GRAVE:
            if(OGKConsole::getSingletonPtr()) {
                OGKConsole::getSingletonPtr()->setVisible(true);
            }
            break;
        case OIS::KC_C:
        {
            if(mCamera->getMode() == OGKCamera::FREE) {
                mCamera->setMode(OGKCamera::THIRD_PERSON_INDIRECT);
                if(mPlayer) mPlayer->setEnabled(true);
                
                // show the mouse
                OGKInputManager::getSingletonPtr()->setMouseVisible(true);
            }
            else {
                mCamera->setMode(OGKCamera::FREE);
                if(mPlayer) mPlayer->setEnabled(false);
                OGKInputManager::getSingletonPtr()->setMouseVisible(false);
            }
            break;
        }
        case OIS::KC_ESCAPE: mShutdown = true; break;
        case OIS::KC_R: _loadGameConfig(); break;
        case OIS::KC_M:
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
            break;
        }
        case OIS::KC_SYSRQ:
            mRenderWindow->writeContentsToTimestampedFile("OGK_", ".png");
            break;
        default:
            break;
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
    if(mCamera && mCamera->getMode() == OGKCamera::THIRD_PERSON_INDIRECT && mTerrain) {
        if(evt.state.buttonDown(OIS::MB_Left)) {
            Ogre::Real x = (float)evt.state.X.abs / (float)evt.state.width;
            Ogre::Real y = (float)evt.state.Y.abs / (float)evt.state.height;
            
            //const OIS::MouseState &mouseState = OGKInputManager::getSingletonPtr()->getMouse()->getMouseState();
            
//            mLog->logMessage("click abs " +
//                               Ogre::StringConverter::toString(evt.state.X.abs) + " " +
//                               Ogre::StringConverter::toString(evt.state.Y.abs) + " rel " +
//                               Ogre::StringConverter::toString(x) + " " +
//                               Ogre::StringConverter::toString(y));
            // set the player destination
            Ogre::Ray ray = mCamera->getCamera()->getCameraToViewportRay(x,y);
            TerrainGroup::RayResult rayResult=mTerrain->mTerrainGroup->rayIntersects(ray);
            if(rayResult.hit) {
                mPlayer->setDestination(rayResult.position);
            }
        }
    }
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKGame::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mCamera && mCamera->getMode() == OGKCamera::THIRD_PERSON_INDIRECT && mTerrain) {
        
        Ogre::Real x = (float)evt.state.X.abs / (float)evt.state.width;
        Ogre::Real y = (float)evt.state.Y.abs / (float)evt.state.height;
        
        //const OIS::MouseState &mouseState = OGKInputManager::getSingletonPtr()->getMouse()->getMouseState();
        
//        mLog->logMessage("click abs " +
//                           Ogre::StringConverter::toString(evt.state.X.abs) + " " +
//                           Ogre::StringConverter::toString(evt.state.Y.abs) + " rel " +
//                           Ogre::StringConverter::toString(x) + " " +
//                           Ogre::StringConverter::toString(y));
        // set the player destination
        Ogre::Ray ray = mCamera->getCamera()->getCameraToViewportRay(x,y);
        TerrainGroup::RayResult rayResult=mTerrain->mTerrainGroup->rayIntersects(ray);
        if(rayResult.hit) {
            mPlayer->setDestination(rayResult.position);
        }
    }
    
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
bool OGKGame::_init(Ogre::String wndTitle)
{
    new Ogre::LogManager();
    mLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log",
                                                        true, true, false);
    mLog->setDebugOutputEnabled(true);
    
    // GAME CONFIG
    _loadGameConfig();
    
    String pluginsPath;
    // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
    pluginsPath = mResourcePath + "plugins.cfg";
#endif
    
    mRoot = new Ogre::Root(pluginsPath, mResourcePath + "ogre.cfg");
    
#ifdef OGRE_STATIC_LIB
    mStaticPluginLoader.load();
#endif
    
    mRoot->restoreConfig();
    
    // RENDER SYSTEM
    _initRenderSystem();
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    mRoot->getRenderSystem()->setConfigOption("macAPI","cocoa");
#endif
	mRenderWindow = mRoot->initialise(true, wndTitle);
    
	mSceneManager = mRoot->createSceneManager(ST_GENERIC, "SceneManager");
    
	// INPUT
    _initInput();
    
    // CAMERA (after input)
    mCamera = OGRE_NEW OGKCamera(mSceneManager, mRenderWindow);
    
    // create overlay system BEFORE initializing resources (for fonts)
    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneManager->addRenderQueueListener(mOverlaySystem);
    
    // RESOURCES
    _initResources();
    
    // GUI
    OGRE_NEW Gorilla::Silverback();
    Gorilla::Silverback::getSingletonPtr()->loadAtlas("dejavu");
    mOverlayScreen = Gorilla::Silverback::getSingletonPtr()->createScreen(mRenderWindow->getViewport(0), "dejavu");
    
    // OVERLAYS
    _initOverlays();
    
    // CONSOLE
    _initConsole();

	mTimer = OGRE_NEW Ogre::Timer();
	mTimer->reset();
    
	mRenderWindow->setActive(true);
    
    mGameSceneManager = OGRE_NEW OGKSceneManager();
    
	return true;
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::_initConsole()
{
    OGRE_NEW OGKConsole();
    OGKConsole::getSingletonPtr()->init(mOverlayScreen);
    OGKConsole::getSingletonPtr()->setVisible(false);
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::_initInput()
{
    OGKInputManager::getSingletonPtr()->initialise(mRenderWindow);
    
    OGKInputManager::getSingletonPtr()->addKeyListener(this, "OGKGameListener");
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->addMultiTouchListener(this, "OGKGameListener");
#else
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "OGKGameListener");
#endif
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::_initRenderSystem()
{
    const Ogre::RenderSystemList *renderSystems = NULL;
    Ogre::RenderSystemList::iterator r_it;
    renderSystems = &mRoot->getAvailableRenderers();
    
    // use first available if any
    if(renderSystems && renderSystems->size()) {
        mRoot->setRenderSystem(renderSystems->front());
    }
    else {
        mLog->logMessage("No available render systems found");
        mRoot->showConfigDialog();
    }
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::_initResources()
{
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
    cf.load(mResourcePath + "resources.cfg");
    
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
                archName = Ogre::String(mResourcePath + archName);
#endif
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void OGKGame::_initRTTTest()
{
    Ogre::TextureManager *mgr = Ogre::TextureManager::getSingletonPtr();
    Ogre::TexturePtr tex = mgr->createManual("RTTTex",
                                           Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                           Ogre::TEX_TYPE_2D,
                                           mRenderWindow->getWidth(),
                                           mRenderWindow->getHeight(),
                                           0,
                                           Ogre::PF_R8G8B8,
                                           Ogre::TU_RENDERTARGET);
    
    Ogre::RenderTexture *rt = tex->getBuffer()->getRenderTarget();
    rt->setAutoUpdated(true);
//    mRenderTexture->addListener(this);
    
    Ogre::Camera *rttCam = mSceneManager->createCamera("RTTCam");
    rttCam->setNearClipDistance(1);
    rttCam->setFarClipDistance(11000);
    
    Ogre::SceneNode *camNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    camNode->attachObject(rttCam);
    
    Ogre::Viewport *vp = rt->addViewport(rttCam);
    rttCam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 0.5f));
    vp->setClearEveryFrame(true);
    vp->setSkiesEnabled(true);
    vp->setOverlaysEnabled(true);
    vp->setCamera(rttCam);

    
//    Gorilla::Screen *screen = Gorilla::Silverback::getSingletonPtr()->createScreen(mCamera->getCamera()->getViewport(), "dejavu");

//    mCamera->getCamera()->setPosition(0,0,0);

    Ogre::SceneNode *rn = mSceneManager->getRootSceneNode()->createChildSceneNode("ScreenRenderableNode");
#define USE_SCREENRENDERABLE2D 1
#ifdef USE_SCREENRENDERABLE2D
    Gorilla::ScreenRenderable2D *sr = Gorilla::Silverback::getSingletonPtr()->createScreenRenderable2D(vp,"dejavu");
    rn->attachObject(sr);
    Gorilla::Layer *layer = sr->createLayer();
#else
    Gorilla::Screen *screen = Gorilla::Silverback::getSingletonPtr()->createScreen(vp, "dejavu");
    rn->attachObject(screen);
    Gorilla::Layer *layer = screen->createLayer();
#endif
    
    Gorilla::Rectangle *r = layer->createRectangle(0,0,1000,100);
    r->background_colour(Ogre::ColourValue::Red);
    
    //Gorilla::Caption *c = layer->createCaption(24, 16, 0, "it works");
    
        // Rect
    Ogre::Rectangle2D *rect = OGRE_NEW Ogre::Rectangle2D(true);
    rect->setCorners(0.0f,0.0f,1.f,-1.f);
    rect->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
    rect->setRenderQueueGroup(RENDER_QUEUE_OVERLAY + 1);
    
    // Scene node
    Ogre::SceneNode *node = mSceneManager->getRootSceneNode()->createChildSceneNode("RTTNode");
    node->attachObject(rect);
        
    // Material
    Ogre::MaterialManager *matMgr = Ogre::MaterialManager::getSingletonPtr();
    Ogre::MaterialPtr mat = matMgr->create("RTTMat",
                                        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    mat->setDepthCheckEnabled(false);
    mat->setDepthWriteEnabled(false);
    mat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    mat->getTechnique(0)->getPass(0)->createTextureUnitState("RTTTex");
    rect->setMaterial("RTTMat");
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::_initOverlays()
{
    
    Gorilla::Layer *overlayLayer = mOverlayScreen->createLayer(14);
    mFPS = overlayLayer->createCaption(14,10,10,Ogre::StringUtil::BLANK);
}

////////////////////////////////////////////////////////////////////////////////
void OGKGame::_loadGameConfig()
{
    mLog->logMessage("Loading game.cfg...");
    if(!mConfig) {
        mConfig = OGRE_NEW Ogre::ConfigFile();
    }
    mConfig->load(mResourcePath + "game.cfg");
    
    // CAMERA
    if(mCamera) {
        mCamera->loadFromConfig();
    }

    // PLAYER
    if(mPlayer) {
        mPlayer->loadFromConfig();
    }
    
    // @TODO audio volumes

    
}
