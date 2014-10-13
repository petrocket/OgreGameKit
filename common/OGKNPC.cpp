//
//  OGKNPC.cpp
//  OgreGameKit
//
//  Created by Alex on 7/2/14.
//
//

#include "OGKNPC.h"
#include "OGKGame.h"
#include "OGKScene.h"
#include "OGKPlayer.h"
#include "Gui3D.h"
#include "Gorilla.h"

OGKNPC::OGKNPC(Ogre::SceneManager *sceneManager) :
    mScene(NULL)
{
    mEntity = sceneManager->createEntity("NPC", "NPC.mesh");
    
    mSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    mSceneNode->attachObject(mEntity);
    mSceneNode->scale(5.0,5.0,5.0);
    
    snapToTerrain();
}

OGKNPC::OGKNPC(Ogre::Entity *entity, Ogre::SceneNode *sceneNode) :
    mEntity(entity),
    mSceneNode(sceneNode),
    mScene(NULL)
{    
    _init();
}

OGKNPC::~OGKNPC()
{
    if(mSceneNode) mSceneNode->detachAllObjects();
}

Ogre::Real OGKNPC::getHealth()
{
    return mHealth;
}

void OGKNPC::heal(Ogre::Real amount)
{
    mHealth += amount;
    mHealth = MAX(0,MIN(mMaxHealth,mHealth));
    _updateHealthBar();
}

void OGKNPC::damage(Ogre::Real amount)
{
    bool justDied = (mHealth > 0.001) && (mHealth - amount <= 0.001);
    
    mHealth -= amount;
    mHealth = MAX(0,MIN(mMaxHealth,mHealth));
    
    if(justDied) {
        if(mBadge) mBadge->setVisible(false);
        mSceneNode->roll(Ogre::Radian(Ogre::Degree(90.0)));
    }
    
    _updateHealthBar();
}

void OGKNPC::setHealth(Ogre::Real amount, Ogre::Real maxAmount)
{
    mHealth = amount;
    mMaxHealth = maxAmount;
    _updateHealthBar();
}

bool OGKNPC::isEnemy()
{
    return mIsEnemy;
}

void OGKNPC::setIsEnemy(bool isEnemy)
{
    mIsEnemy = isEnemy;
    
    if(mBadge) {
        if(isEnemy) {
            mBadge->setHealthBarColour(Ogre::ColourValue(0.5,0,0,1));
            mBadge->setHealthBarBorderColour(Ogre::ColourValue(0.25,0,0,1));
        }
        else {
            mBadge->setHealthBarColour(Ogre::ColourValue(0.0,0.5,0,1));
            mBadge->setHealthBarBorderColour(Ogre::ColourValue(0,0.25,0,1));
        }
    }
}

void OGKNPC::snapToTerrain()
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

void OGKNPC::update(Ogre::Real elapsedTime)
{
    if(mHealth > 0.001) {
        
        /*
        OGKPlayer *player = mScene ? mScene->getPlayer() : NULL;
        if(mIsEnemy && player) {
            Ogre::Vector3 dir = player->getSceneNode()->getPosition() -
                                mSceneNode->getPosition();
            
            if(dir.squaredLength() < 1000.0) {
                // face the player
                Ogre::Real dist = dir.normalise();
                Ogre::Real moveScale = mMoveSpeed   * (float)elapsedTime;
                Ogre::Vector3 src = mSceneNode->getOrientation() * -Ogre::Vector3::UNIT_Z;
                
                // keep the player upright
                Ogre::Vector3 rotateDir(dir.x, 0, dir.z);
                rotateDir.normalise();
                
                Ogre::Quaternion quat = src.getRotationTo(rotateDir);
                mSceneNode->rotate(quat);
                
                // move to the enemy
                if(dist > 3.0) {
                    mSceneNode->translate(dir * moveScale);
                }
                else if(mAttackCooldown <= 0) {
                    player->damage(4);
                    mAttackCooldown = mAttackSpeed;
                }
            }
        }
         */
        
        mAttackCooldown -= elapsedTime;
        
        snapToTerrain();
    }
}

void OGKNPC::_init()
{
    mAttackSpeed = 500;
    mAttackCooldown = 0;
    mBadge      = NULL;
    mEnabled    = true;
    mHealth     = 100.0;
    mMaxHealth  = 100.0;
    mIsEnemy    = false;
    mMoveSpeed  = 0.01;
    mMovingState = OGKNPC::NONE;
    mRotateSpeed = 1.0;
    
    OGKSceneManager *gameSceneManager = OGKGame::getSingletonPtr()->mGameSceneManager;
    mScene = gameSceneManager->getActiveScene();
    
    // health bar
    //_initHealthBar();
    
    // set bar colours
    setIsEnemy(mIsEnemy);
    
#ifdef OGRE_IS_IOS
    mEntity->setMaterialName("OGK/Debug/Gray");
#endif
    
    if(mEntity->hasAnimationState("Walk")) {
        mEntity->getAnimationState("Walk")->setLoop(true);
        mEntity->getAnimationState("Walk")->setEnabled(false);
    }
    if(mEntity->hasAnimationState("Idle")) {
        mEntity->getAnimationState("Idle")->setLoop(true);
        mEntity->getAnimationState("Idle")->setEnabled(true);
    }
    
    snapToTerrain();
}

void OGKNPC::_initHealthBar()
{
    if(!mScene) {
        OGKLOG("Can't create NPC health bar becuz scene is null");
        return;
    }
    /*
    mBadge = OGRE_NEW OGKBadge(mSceneNode,
                                mScene->getGUI()->getSilverback(),
                                mScene->mCamera->getCamera(),
                                Ogre::Vector3(0,1.0,0));

    mBadge->setHealth(1.0);
    mBadge->setHealthBarColour(Ogre::ColourValue(0.5,0,0,1));
    mBadge->setHealthBarBorderColour(Ogre::ColourValue(0.25,0,0,1));
    mBadge->setCaption("");
     */
}

void OGKNPC::_updateHealthBar()
{
    if(mBadge && mMaxHealth > 0) {
        mBadge->setHealth(mHealth / mMaxHealth);
    }
}
