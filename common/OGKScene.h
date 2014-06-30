//
//  OGKScene.h
//  OgreGameKit
//
//  Created by Alex on 6/24/14.
//
//

#ifndef __OgreGameKit__OGKScene__
#define __OgreGameKit__OGKScene__

#include <Ogre.h>
#include <OgreOverlaySystem.h>

// input
#include <OISKeyboard.h>
#include <OISMouse.h>

class OGKCamera;

// GUI
#include "Gui3D.h"

#ifdef OGRE_IS_IOS
#include <OISMultiTouch.h>
class OGKScene : public OIS::KeyListener, public OIS::MultiTouchListener
#else
class OGKScene : public OIS::KeyListener, public OIS::MouseListener
#endif
{
public:
    OGKScene(const Ogre::String& name);
    ~OGKScene();
    
    virtual void init();
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    
    virtual void update(double timeSinceLastFrame);

    virtual void onExit();
    virtual void onExitTransitionDidStart();

    bool isRunning();
    
    // INPUT
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
    
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
    
    // CAMERA
    OGKCamera *mCamera;
    
    // GUI
    Gorilla::ScreenRenderable2D *mScreen;

    Ogre::String mSceneName;

    Ogre::SceneManager *mSceneManager;
    Ogre::SceneNode *mSceneNode;
    
    Ogre::Overlay *mOverlay;
    Ogre::OverlaySystem *mOverlaySystem;
    
protected:
    bool mRunning;
};

#endif /* defined(__OgreGameKit__OGKScene__) */
