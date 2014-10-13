//
//  OGKNPC.h
//  OgreGameKit
//
//  Created by Alex on 7/2/14.
//
//

#ifndef __OgreGameKit__OGKNPC__
#define __OgreGameKit__OGKNPC__

#include <Ogre.h>
#include "OGKBadge.h"

class OGKScene;

class OGKNPC
{
public:
    OGKNPC(Ogre::SceneManager *sceneManager);
    OGKNPC(Ogre::Entity *entity, Ogre::SceneNode *sceneNode);
    ~OGKNPC();
    
    enum MovingState {
        NONE,
        USER_CONTROLLED,
        START_MOVING,
        MOVING,
        ARRIVED,
        AT_DESTINATION
    };
    
    void snapToTerrain();
    
    Ogre::Entity *mEntity;
    
    bool isEnemy();
    void setIsEnemy(bool isEnemy);
    
    Ogre::Real getHealth();
    void setHealth(Ogre::Real amount, Ogre::Real maxAmount);
    void heal(Ogre::Real amount);
    void damage(Ogre::Real amount);

    void update(Ogre::Real elapsedTime);
    
private:
    void _init();
    void _initHealthBar();
    void _updateHealthBar();
    
    OGKBadge *mBadge;
    
    Ogre::Vector3 mDestination;
    
    bool mEnabled;
    bool mIsEnemy;
    
    Ogre::Real mHealth;
    Ogre::Real mMaxHealth;
    Ogre::Real mAttackSpeed;
    Ogre::Real mAttackCooldown;
    
    MovingState mMovingState;
    
    Ogre::Real mMoveSpeed;
    Ogre::Real mRotateSpeed;
    Ogre::SceneNode *mSceneNode;
    
    OGKScene *mScene;
};

#endif /* defined(__OgreGameKit__OGKNPC__) */
