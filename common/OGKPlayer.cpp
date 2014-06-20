//
//  OGKPlayer.cpp
//  OgreGameKit
//
//  Created by Alex on 6/19/14.
//
//

#include "OGKPlayer.h"
#include "OGKGame.h"
#include "OGKInputManager.h"


OGKPlayer::OGKPlayer(Ogre::SceneManager *sceneManager) :
    mEnabled(true),
    mEntity(NULL),
    mMoveSpeed(1.0),
    mSceneNode(NULL),
    mWalking(false)
{
    mEntity = sceneManager->createEntity("Player", "Player.mesh");
    mSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(mEntity);
    
    OGKTerrain *terrain = OGKGame::getSingletonPtr()->getTerrain();
    if(terrain) {
        float height = terrain->mTerrainGroup->getHeightAtWorldPosition(0, 1000.0, 0);
        mSceneNode->setPosition(Ogre::Vector3(0,height,0));
    }
    
    if(mEntity->hasAnimationState("Walk")) {
        mEntity->getAnimationState("Walk")->setLoop(true);
        mEntity->getAnimationState("Walk")->setEnabled(false);
    }
    if(mEntity->hasAnimationState("Idle")) {
        mEntity->getAnimationState("Idle")->setLoop(true);
        mEntity->getAnimationState("Idle")->setEnabled(true);
    }
    
    OGKInputManager::getSingletonPtr()->addKeyListener(this, "OGKPlayerListener");
    
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->addMultiTouchListener(this, "OGKPlayerListener");
#else
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "OGKPlayerListener");
#endif
}

OGKPlayer::~OGKPlayer()
{
    mSceneNode->detachAllObjects();
}

bool OGKPlayer::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    return TRUE;
}

bool OGKPlayer::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return TRUE;
}

#ifdef OGRE_IS_IOS

bool OGKPlayer::touchMoved(const OIS::MultiTouchEvent &evt)
{
    return TRUE;
}

bool OGKPlayer::touchPressed(const OIS::MultiTouchEvent &evt)
{
    return TRUE;
}

bool OGKPlayer::touchReleased(const OIS::MultiTouchEvent &evt)
{
    return TRUE;
}

bool OGKPlayer::touchCancelled(const OIS::MultiTouchEvent &evt)
{
    return TRUE;
}

#else

bool OGKPlayer::mouseMoved(const OIS::MouseEvent &evt)
{
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
bool OGKPlayer::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKPlayer::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}
#endif

void OGKPlayer::update(Ogre::Real elapsedTime)
{
    bool moving = false;
    
    if(mEnabled) {
        if(mWalking) {
            moving = true;
        }
        else {
            Ogre::Vector3 translateVector(0,0,0);
            Ogre::Real moveScale = mMoveSpeed   * (float)elapsedTime;
#ifndef OGRE_IS_IOS
            OIS::Keyboard *keyboard = OGKInputManager::getSingletonPtr()->getKeyboard();
            
            if(keyboard->isKeyDown(OIS::KC_A))
                translateVector.x = -moveScale;
            
            if(keyboard->isKeyDown(OIS::KC_D))
                translateVector.x = moveScale;
            
            if(keyboard->isKeyDown(OIS::KC_W))
                translateVector.z = -moveScale;
            
            if(keyboard->isKeyDown(OIS::KC_S))
                translateVector.z = moveScale;
            
            if(translateVector.x < -0.01 || translateVector.x > 0.01 ||
               translateVector.z < - 0.01 || translateVector.z > 0.01) {
                
                moving = true;
                
                if(keyboard->isKeyDown(OIS::KC_LSHIFT)) {
                    mSceneNode->translate(translateVector * 0.1f);
                }
                else {
                    mSceneNode->translate(translateVector);
                }
                
                // snap to terrain
                OGKTerrain *terrain = OGKGame::getSingletonPtr()->getTerrain();
                if(terrain) {
                    Ogre::Vector3 pos = mSceneNode->getPosition();
                    float height = terrain->mTerrainGroup->getHeightAtWorldPosition(pos);
                    pos.y = height + 1.f;
                    mSceneNode->setPosition(pos);
                }
            }
#endif
        }
    }
    
    if(mEntity) {
        OIS::Keyboard *keyboard = OGKInputManager::getSingletonPtr()->getKeyboard();
        
        if(mEntity->hasAnimationState("Walk")) {
            mEntity->getAnimationState("Walk")->setEnabled(moving);
            mEntity->getAnimationState("Walk")->addTime(elapsedTime * 0.001);
        }
        if(mEntity->hasAnimationState("Idle")) {
            mEntity->getAnimationState("Idle")->setEnabled(!moving);
            mEntity->getAnimationState("Idle")->addTime(elapsedTime * 0.001);
        }
    }
}

bool OGKPlayer::getEnabled()
{
    return mEnabled;
}

void OGKPlayer::setEnabled(bool enabled)
{
    mEnabled = enabled;
}

Ogre::Vector3 OGKPlayer::getDestination()
{
    return mDestination;
}

void OGKPlayer::setDestination(Ogre::Vector3 destination)
{
    mDestination = destination;
}

Ogre::SceneNode *OGKPlayer::getSceneNode()
{
    return mSceneNode;
}

