//
//  OGKPlayer.h
//  OgreGameKit
//
//  Created by Alex on 6/19/14.
//
//

#ifndef __OgreGameKit__OGKPlayer__
#define __OgreGameKit__OGKPlayer__

#include <Ogre.h>
#include "OGKStaticPluginLoader.h"

// input
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#ifdef OGRE_IS_IOS
#include <OISMultiTouch.h>
class OGKPlayer : public OIS::KeyListener, OIS::MultiTouchListener
#else
class OGKPlayer : public OIS::KeyListener, OIS::MouseListener
#endif
{
public:
    OGKPlayer(Ogre::SceneManager *sceneManager);
    ~OGKPlayer();
    
    enum MovingState {
        NONE,
        USER_CONTROLLED,
        START_MOVING,
        MOVING,
        ARRIVED,
        AT_DESTINATION
    };
    
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
    
    void loadFromConfig();
    
    void update(Ogre::Real elapsedTime);
    
    bool getEnabled();
    void setEnabled(bool enabled);
    
    Ogre::Vector3 getDestination();
    void setDestination(Ogre::Vector3 destination);
    
    Ogre::Real getMoveSpeed();
    void setMoveSpeed(Ogre::Real speed);

    Ogre::Real getRotateSpeed();
    void setRotateSpeed(Ogre::Real speed);

    Ogre::SceneNode *getSceneNode();
    
private:
    Ogre::Entity *mEntity;
    Ogre::SceneNode *mSceneNode;
    
    bool mEnabled;
    MovingState mMovingState;
    Ogre::Real mMoveSpeed;
    Ogre::Real mRotateSpeed;
    Ogre::Vector3 mDestination;
};

#endif /* defined(__OgreGameKit__OGKPlayer__) */
