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
#include "OGKStaticPluginLoader.h"
#include <OgreOverlaySystem.h>

// input
#include <OISKeyboard.h>
#include <OISMouse.h>

class OGKCamera;
class OgreRayCollision;

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
    
    bool isRunning();
    
    virtual void loadFromFile(const Ogre::String& name);
    virtual void loadFromFile(const Ogre::String& name,
                              const std::vector<Ogre::String>& objectsToIgnore);
    virtual void saveToFile(const Ogre::String& name);
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    
    virtual void update(Ogre::Real elapsedTime);

    virtual void onExit();
    virtual void onExitTransitionDidStart();
    
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
    
    virtual bool rayIntersects(const Ogre::Ray& ray, Ogre::Vector3& hitLocation, Ogre::MovableObject *hitObject, bool terrainOnly = false);
    
protected:
    bool mRunning;
    
    virtual Ogre::MovableObject *getTerrainObject();
    
    OgreRayCollision *mCollisionRay;
};

#endif /* defined(__OgreGameKit__OGKScene__) */
