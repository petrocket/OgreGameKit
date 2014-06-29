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

OGKScene::OGKScene(const Ogre::String& name) :
    mCamera(NULL),
    mRunning(false),
    mScreen(NULL),
    mSceneName(name),
    mSceneNode(NULL)
{
    init();
}

OGKScene::~OGKScene()
{
    
}

void OGKScene::init()
{
}

bool OGKScene::isRunning()
{
    return mRunning;
}

void OGKScene::onEnter()
{
    Ogre::SceneManager *mgr = OGKGame::getSingletonPtr()->mSceneManager;

    // scene node gets deleted
    if(!mSceneNode) {
        mSceneNode = mgr->getRootSceneNode()->createChildSceneNode("OGKScene" + mSceneName);
    }
    
    if(!mCamera) {
        mCamera = OGRE_NEW OGKCamera(mgr, OGKGame::getSingletonPtr()->mRenderWindow);
    }
    else {
        mCamera->setEnabled(true);
    }
}

void OGKScene::onEnterTransitionDidFinish()
{
    
}

void OGKScene::update(double timeSinceLastFrame)
{
    
}

void OGKScene::onExit()
{
    if(mSceneNode) {
        mSceneNode->detachAllObjects();
        
        Ogre::SceneManager *mgr = OGKGame::getSingletonPtr()->mSceneManager;
        mgr->getRootSceneNode()->removeAndDestroyChild(mSceneNode->getName());

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

