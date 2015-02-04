//
//  OGKScene.cpp
//  OgreGameKit
//
//  Created by Alex on 6/24/14.
//
//

#include "OGKScene.h"
#include "OGKCamera.h"
#include "OGKGame.h"

// dot scene loader
#include "DotSceneLoader.h"

// collision
#include "OgreRayCollision.h"

#ifdef INCLUDE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"
#endif

OGKScene::OGKScene(const Ogre::String& name) :
    mCamera(NULL),
    mCollisionRay(NULL),
    mRunning(false),
    mScreen(NULL),
    mSceneName(name),
    mSceneNode(NULL)
{
    // this will call OGKScene:init (not sub class init)
    init();
}

OGKScene::~OGKScene()
{
#ifdef INCLUDE_RTSHADER_SYSTEM
    Ogre::RTShader::ShaderGenerator::getSingletonPtr()->removeSceneManager(mSceneManager);
#endif
    
    Ogre::Root::getSingletonPtr()->destroySceneManager(mSceneManager);
}

Ogre::MovableObject *OGKScene::getTerrainObject()
{
    // for subclasses
    return NULL;
}

void OGKScene::init()
{
    mSceneManager =  Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC, "SceneManager" + mSceneName);
    mSceneManager->addRenderQueueListener(OGKGame::getSingletonPtr()->mOverlaySystem);
    
#ifdef INCLUDE_RTSHADER_SYSTEM
    Ogre::RTShader::ShaderGenerator::getSingletonPtr()->addSceneManager(mSceneManager);
#endif
    
    mOverlay = Ogre::OverlayManager::getSingletonPtr()->create("SceneOverlay" + mSceneName);
    mOverlay->setZOrder(100);
}

bool OGKScene::isRunning()
{
    return mRunning;
}

void OGKScene::loadFromFile(const Ogre::String& name)
{
    loadFromFile(name, std::vector<Ogre::String>());
}

void OGKScene::loadFromFile(const Ogre::String& name, const std::vector<Ogre::String>& objectsToIgnore)
{
    // does this resource exist?
    DotSceneLoader *loader = OGRE_NEW DotSceneLoader();
    loader->objectNamesToIgnore = objectsToIgnore;
    if(!loader->parseDotScene(name, "General", mSceneManager)) {
        OGKLOG("Failed to parse dot scene file " + name);
    }
    
    OGRE_DELETE loader;
}

void OGKScene::saveToFile(const Ogre::String& name)
{
    // @TODO
}

void OGKScene::onEnter()
{
    // scene node gets deleted on exit, so re-create if needed
    if(!mSceneNode) {
        mSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode("OGKScene" + mSceneName);
    }
    
    if(!mCamera) {
        mCamera = OGRE_NEW OGKCamera(mSceneManager, OGKGame::getSingletonPtr()->mRenderWindow);
    }
    else {
        mCamera->setEnabled(true);
    }
    
    mRunning = true;
}

void OGKScene::onEnterTransitionDidFinish()
{
#ifndef OGRE_IS_IOS
    // get the mouse in the right position
    OIS::MouseEvent evt(NULL, OGKInputManager::getSingletonPtr()->getMouse()->getMouseState());
    mouseMoved(evt);
#endif
}

void OGKScene::update(Ogre::Real elapsedTime)
{
    if(mCamera) mCamera->update(elapsedTime);
}

void OGKScene::onExit()
{
    if(mSceneNode) {
        mSceneManager->clearScene();
        mSceneNode = NULL;
    }
    
    if(mCamera) {
        mCamera->setEnabled(false);
    }
}

void OGKScene::onExitTransitionDidStart()
{
    mRunning = false;
}

bool OGKScene::keyPressed(const OIS::KeyEvent &keyEventRef) { return true; }
bool OGKScene::keyReleased(const OIS::KeyEvent &keyEventRef)  { return true; }

#ifdef OGRE_IS_IOS
bool OGKScene::touchMoved(const OIS::MultiTouchEvent &evt)  { return true; }
bool OGKScene::touchPressed(const OIS::MultiTouchEvent &evt)  { return true; }
bool OGKScene::touchReleased(const OIS::MultiTouchEvent &evt)  { return true; }
bool OGKScene::touchCancelled(const OIS::MultiTouchEvent &evt)  { return true; }
#else
bool OGKScene::mouseMoved(const OIS::MouseEvent &evt)  { return true; }
bool OGKScene::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)  { return true; }
bool OGKScene::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)  { return true; }
#endif

bool OGKScene::rayIntersects(const Ogre::Ray& ray, Ogre::Vector3& hitLocation, Ogre::MovableObject *hitObject, bool terrainOnly)
{
    if(!mCollisionRay) {
        mCollisionRay = OGRE_NEW OgreRayCollision(mSceneManager);
    }
    
    if(terrainOnly) {
        hitObject = getTerrainObject();
        return hitObject ? mCollisionRay->RaycastObject( ray.getOrigin(), ray.getDirection(), hitLocation, hitObject) : false;
    }
    else {
        return mCollisionRay->RaycastFromPoint( ray.getOrigin(), ray.getDirection(), hitLocation, hitObject);

    }
}


