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
#include "DotSceneLoader.h"

OGKScene::OGKScene(const Ogre::String& name) :
    mCamera(NULL),
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
    Ogre::Root::getSingletonPtr()->destroySceneManager(mSceneManager);
}

void OGKScene::init()
{
    mSceneManager =  Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC, "SceneManager" + mSceneName);
    mSceneManager->addRenderQueueListener(OGKGame::getSingletonPtr()->mOverlaySystem);
    
    mOverlay = Ogre::OverlayManager::getSingletonPtr()->create("SceneOverlay" + mSceneName);
}

bool OGKScene::isRunning()
{
    return mRunning;
}

void OGKScene::loadFromFile(const Ogre::String& name)
{
    // does this resource exist?
    DotSceneLoader *loader = OGRE_NEW DotSceneLoader();
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

