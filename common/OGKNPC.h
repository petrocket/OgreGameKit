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

class OGKInGameScene;

class OGKNPC
{
public:
    OGKNPC(Ogre::SceneManager *sceneManager);
    OGKNPC(Ogre::Entity *entity, Ogre::SceneNode *sceneNode);
    ~OGKNPC();
    
    void update(Ogre::Real elapsedTime);
    
    void snapToTerrain();
    
    Ogre::Entity *mEntity;
private:
    
    Ogre::Vector3 mDestination;
    bool mEnabled;
    Ogre::Real mMoveSpeed;
    Ogre::Real mRotateSpeed;
    Ogre::SceneNode *mSceneNode;
    
    OGKInGameScene *mScene;
};

#endif /* defined(__OgreGameKit__OGKNPC__) */
