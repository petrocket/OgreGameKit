//
//  OGKGame.h
//  OgreGameKit
//
//  Created by Alex on 5/20/14.
//
//

#ifndef __OgreGameKit__OGKGame__
#define __OgreGameKit__OGKGame__

// gfx
#include <Ogre.h>
#include <OgreOverlaySystem.h>

// input
//#include <OISEvents.h>
//#include <OISInputManager.h>
//#include <OISKeyboard.h>
//#include <OISMouse.h>
#include "OGKInputManager.h"

// defines
#include "OGKStaticPluginLoader.h"


// camera
#include "OGKCamera.h"

// audio
#include "OGKAudio.h"

// scene
#include "OGKScene.h"
#include "OGKSceneManager.h"

namespace Gorilla {
    class Caption;
    class Screen;
}

#define OGKLOG(m) OGKGame::getSingletonPtr()->mLog->logMessage(m)

#ifdef OGRE_IS_IOS
#include <OISMultiTouch.h>
class OGKGame : public Ogre::Singleton<OGKGame>, OIS::KeyListener, OIS::MultiTouchListener
#else
class OGKGame : public Ogre::Singleton<OGKGame>, OIS::KeyListener, OIS::MouseListener
#endif
{
public:
	OGKGame();
	~OGKGame();
    
	bool isOgreToBeShutDown()const{return mShutdown;}
    
    Ogre::ConfigFile* getGameConfig();
    
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
    
    bool renderOneFrame();
    bool renderOneFrame(double timeSinceLastFrame);
    void setup();
    void shutdown();
    void start();
    void update(double timeSinceLastFrame);
    
#ifdef OGRE_IS_IOS
	bool touchMoved(const OIS::MultiTouchEvent &evt);
	bool touchPressed(const OIS::MultiTouchEvent &evt);
	bool touchReleased(const OIS::MultiTouchEvent &evt);
	bool touchCancelled(const OIS::MultiTouchEvent &evt);
#else
	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
#endif
	
	Ogre::Root*                 mRoot;
	Ogre::SceneManager*			mSceneManager;
	Ogre::RenderWindow*			mRenderWindow;
	OGKCamera*                  mCamera;
	Ogre::Log*                  mLog;
	Ogre::Timer*				mTimer;
    Ogre::OverlaySystem*        mOverlaySystem;
    
    OGKSceneManager*            mGameSceneManager;
    
#ifdef OGRE_IS_IOS
    Ogre::OrientationMode       mViewportOrientation;
#endif
    
protected:
    Ogre::String                 mResourcePath;
    Ogre::ConfigFile            *mConfig;
    
private:
	OGKGame(const OGKGame&);
	OGKGame& operator= (const OGKGame&);
    
    bool _init(Ogre::String wndTitle);
    
    void _initInput();
    void _initConsole();
    void _initRenderSystem();
    void _initResources();
    void _initOverlays();
    void _loadGameConfig();
    
    // fps
    Gorilla::Screen*  mOverlayScreen;
    Gorilla::Caption* mFPS;
    
	double                      mTimeSinceLastFrame;
	double                      mStartTime;
	bool                        mShutdown;
    
#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader    mStaticPluginLoader;
#endif
};

#endif
