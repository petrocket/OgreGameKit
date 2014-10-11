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
#include "OGKTerrain.h"
#include "OGKScene.h"

OGKPlayer::OGKPlayer(Ogre::SceneManager *sceneManager) :
    mEnabled(true),
    mEntity(NULL),
    mMoveSpeed(1.0),
    mMovingState(OGKPlayer::NONE),
    mRotateSpeed(1.0),
    mSceneManager(sceneManager),
    mSceneNode(NULL),
    mScene(NULL)
{
    OGKInputManager::getSingletonPtr()->addKeyListener(this, "OGKPlayerListener");
    
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->addMultiTouchListener(this, "OGKPlayerListener");
#else
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "OGKPlayerListener");
#endif
    
    init();
}

OGKPlayer::~OGKPlayer()
{
    mSceneNode->detachAllObjects();
    
    OGKInputManager::getSingletonPtr()->removeKeyListener(this);
    
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->removeMultiTouchListener(this);
#else
    OGKInputManager::getSingletonPtr()->removeMouseListener(this);
#endif
}

void OGKPlayer::attack(Ogre::Vector3 position)
{
    // @TODO
}

void OGKPlayer::attack(Ogre::Entity *entity)
{
    // @TODO
}

void OGKPlayer::init()
{
    mEntity = mSceneManager->createEntity("Player", "Player.mesh");
    
#ifdef OGRE_IS_IOS
    mEntity->setMaterialName("OGK/Debug/Gray");
#endif
    
    mSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(mEntity);
    mSceneNode->scale(5.0,5.0,5.0);
    
    OGKSceneManager *gameSceneManager = OGKGame::getSingletonPtr()->mGameSceneManager;
    mScene = gameSceneManager->getActiveScene();
    
    //    if(mScene) {
    //        OGKTerrain *terrain = mScene->getTerrain();
    //        if(NULL != terrain) {
    //            float height = terrain->mTerrainGroup->getHeightAtWorldPosition(0, 1000.0, 0);
    //            mSceneNode->setPosition(Ogre::Vector3(0,height,0));
    //        }
    //    }
    
    snapToTerrain();
    
    if(mEntity->hasAnimationState("Walk")) {
        mEntity->getAnimationState("Walk")->setLoop(true);
        mEntity->getAnimationState("Walk")->setEnabled(false);
    }
    if(mEntity->hasAnimationState("Idle")) {
        mEntity->getAnimationState("Idle")->setLoop(true);
        mEntity->getAnimationState("Idle")->setEnabled(true);
    }
    
    loadFromConfig();
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

////////////////////////////////////////////////////////////////////////////////
void OGKPlayer::loadFromConfig()
{
    Ogre::ConfigFile *cf = OGKGame::getSingletonPtr()->getGameConfig();
    if(cf) {
        Ogre::String setting;
        setting = cf->getSetting("moveSpeed","player");
        if(setting.length()) {
            setMoveSpeed(Ogre::StringConverter::parseReal(setting));
        }
        setting = cf->getSetting("rotateSpeed","player");
        if(setting.length()) {
            setRotateSpeed(Ogre::StringConverter::parseReal(setting));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void OGKPlayer::update(Ogre::Real elapsedTime)
{
    bool moving = false;
    
    if(!isAlive()) {
        // do nothing
    }
    else if(mMovingState == USER_CONTROLLED) {
        Ogre::Vector3 translateVector(0,0,0);
        Ogre::Real rotateAmount = 0;
        Ogre::Real moveScale = mMoveSpeed   * (float)elapsedTime;
        Ogre::Real rotateScale = mRotateSpeed   * (float)elapsedTime;
#ifndef OGRE_IS_IOS
        OIS::Keyboard *keyboard = OGKInputManager::getSingletonPtr()->getKeyboard();
        
        if(keyboard->isKeyDown(OIS::KC_A))
            rotateAmount = rotateScale;
        
        if(keyboard->isKeyDown(OIS::KC_D))
            rotateAmount = -rotateScale;
        
        if(keyboard->isKeyDown(OIS::KC_W))
            translateVector.z = -moveScale;
        
        if(keyboard->isKeyDown(OIS::KC_S))
            translateVector.z = moveScale;
        
        if(rotateAmount > 0.01 || rotateAmount < -0.01) {
            mSceneNode->yaw(Ogre::Radian(rotateAmount));
        }
        
        if(translateVector.x < -0.01 || translateVector.x > 0.01 ||
           translateVector.z < - 0.01 || translateVector.z > 0.01) {
            
            moving = true;
            
            if(keyboard->isKeyDown(OIS::KC_LSHIFT)) {
                mSceneNode->translate(translateVector * 0.1f,Ogre::SceneNode::TS_LOCAL);
            }
            else {
                mSceneNode->translate(translateVector,Ogre::SceneNode::TS_LOCAL);
            }
            
            // snap to terrain
            snapToTerrain();
        }
#endif
    }
    else if(mMovingState != OGKPlayer::AT_DESTINATION &&
           mMovingState != OGKPlayer::NONE) {

        if(mMovingState == OGKPlayer::ARRIVED) {
            mMovingState = OGKPlayer::AT_DESTINATION;
        }
        // check distance ignoring height
        else if(Ogre::Vector3(mDestination.x,0,mDestination.z).squaredDistance(Ogre::Vector3(mSceneNode->getPosition().x,0,mSceneNode->getPosition().z)) < 2.f) {
            mMovingState = OGKPlayer::ARRIVED;
        }
        else {
            Ogre::Vector3 dir = mDestination - mSceneNode->getPosition();
            Ogre::Real dist = dir.normalise();
            Ogre::Real moveScale = mMoveSpeed   * (float)elapsedTime;
            
            if(mMovingState == OGKPlayer::START_MOVING) {
                Ogre::Vector3 src = mSceneNode->getOrientation() * -Ogre::Vector3::UNIT_Z;
                
                // keep the player upright
                Ogre::Vector3 rotateDir(dir.x, 0, dir.z);
                rotateDir.normalise();
                
                Ogre::Quaternion quat = src.getRotationTo(rotateDir);
                mSceneNode->rotate(quat);
            }
            
            mMovingState = OGKPlayer::MOVING;
            
            if(moveScale > dist) {
                // translate to the exact position
                mSceneNode->setPosition(mDestination);
                mMovingState = OGKPlayer::ARRIVED;
            }
            else {
                mSceneNode->translate(dir * moveScale);
            }
        }
        
        moving = mMovingState != OGKPlayer::AT_DESTINATION;
        
        if(moving) {
            snapToTerrain();
        }
    }
    else {
        snapToTerrain();
    }
    
    if(mEntity) {
        OIS::Keyboard *keyboard = OGKInputManager::getSingletonPtr()->getKeyboard();
        
        if(mEntity->hasAnimationState("Walk")) {
            mEntity->getAnimationState("Walk")->setEnabled(moving);
            mEntity->getAnimationState("Walk")->addTime(elapsedTime * 0.15 * mMoveSpeed);
        }
        if(mEntity->hasAnimationState("Idle")) {
            mEntity->getAnimationState("Idle")->setEnabled(!moving);
            mEntity->getAnimationState("Idle")->addTime(elapsedTime * 0.001);
        }
    }
}

void OGKPlayer::snapToTerrain()
{
    if(mScene) {
        Ogre::Vector3 pos = mSceneNode->getPosition();
        Ogre::Ray ray(pos + Ogre::Vector3(0,100.f,0), Ogre::Vector3::NEGATIVE_UNIT_Y);
        Ogre::Vector3 hitLocation = Ogre::Vector3::ZERO;
        Ogre::MovableObject *hitObject = NULL;
        bool terrainOnly = true;
        
        // terrain only
        if(mScene->rayIntersects(ray, hitLocation, hitObject, terrainOnly)) {
            pos.y = hitLocation.y + 0.1f;
            mSceneNode->setPosition(pos);
        }
/*
        OGKTerrain *terrain = mScene->getTerrain();
        if(terrain) {
            Ogre::Vector3 pos = mSceneNode->getPosition();
            float height = terrain->mTerrainGroup->getHeightAtWorldPosition(pos);
            pos.y = height + 0.1f;
            mSceneNode->setPosition(pos);
        }
 */
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

void OGKPlayer::setHealth(Ogre::Real amount, Ogre::Real maxAmount)
{
    mHealth = amount;
    mMaxHealth = maxAmount;
}

void OGKPlayer::heal(Ogre::Real amount)
{
    mHealth += amount;
    mHealth = MAX(0,MIN(mMaxHealth,mHealth));
}

void OGKPlayer::damage(Ogre::Real amount)
{
    bool justDied = (mHealth > 0.001) && (mHealth - amount <= 0.001);
    
    mHealth -= amount;
    mHealth = MAX(0,MIN(mMaxHealth,mHealth));
    
    if(justDied) {
        mSceneNode->roll(Ogre::Radian(Ogre::Degree(90.0)));
    }
}

Ogre::Vector3 OGKPlayer::getDestination()
{
    return mDestination;
}

void OGKPlayer::setDestination(Ogre::Vector3 destination)
{
    mDestination = destination;
    mMovingState = OGKPlayer::START_MOVING;
}

Ogre::SceneNode *OGKPlayer::getSceneNode()
{
    return mSceneNode;
}

Ogre::Real OGKPlayer::getMoveSpeed()
{
    return mMoveSpeed;
}

void OGKPlayer::setMoveSpeed(Ogre::Real speed)
{
    mMoveSpeed = speed;
}

Ogre::Real OGKPlayer::getRotateSpeed()
{
    return mRotateSpeed;
}

void OGKPlayer::setRotateSpeed(Ogre::Real speed)
{
    mRotateSpeed = speed;
}

