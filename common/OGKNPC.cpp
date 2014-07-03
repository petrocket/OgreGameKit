//
//  OGKNPC.cpp
//  OgreGameKit
//
//  Created by Alex on 7/2/14.
//
//

#include "OGKNPC.h"
#include "OGKGame.h"
#include "OGKInGameScene.h"

OGKNPC::OGKNPC(Ogre::SceneManager *sceneManager) :
    mEnabled(true),
    mEntity(NULL),
    mMoveSpeed(1.0),
    mRotateSpeed(1.0),
    mSceneNode(NULL)
{
    mEntity = sceneManager->createEntity("NPC", "NPC.mesh");
    mSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(mEntity);
    mSceneNode->scale(5.0,5.0,5.0);
    
    OGKSceneManager *gameSceneManager = OGKGame::getSingletonPtr()->mGameSceneManager;
    mScene = (OGKInGameScene *)gameSceneManager->getScene("ingame");
    
    snapToTerrain();
}

OGKNPC::OGKNPC(Ogre::Entity *entity, Ogre::SceneNode *sceneNode) :
    mEntity(entity),
    mSceneNode(sceneNode)
{    
    OGKSceneManager *gameSceneManager = OGKGame::getSingletonPtr()->mGameSceneManager;
    mScene = (OGKInGameScene *)gameSceneManager->getScene("ingame");
    
    snapToTerrain();
}

OGKNPC::~OGKNPC()
{
    if(mSceneNode) mSceneNode->detachAllObjects();
}

void OGKNPC::snapToTerrain()
{
    if(mScene) {
        OGKTerrain *terrain = mScene->getTerrain();
        if(terrain) {
            Ogre::Vector3 pos = mSceneNode->getPosition();
            float height = terrain->mTerrainGroup->getHeightAtWorldPosition(pos);
            pos.y = height + 0.1f;
            mSceneNode->setPosition(pos);
        }
    }
}

void OGKNPC::update(Ogre::Real elapsedTime)
{
    snapToTerrain();
}
