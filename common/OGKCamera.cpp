//
//  OGKCamera.cpp
//  OgreGameKit
//
//  Created by Alex on 6/19/14.
//
//

#include "OGKCamera.h"
#include "OGKInputManager.h"

OGKCamera::OGKCamera(
                     Ogre::SceneManager *sceneManager,
                     Ogre::RenderWindow *renderWindow) :
    mCamera(NULL),
    mMode(FREE),
    mMoveSpeed(1.0),
    mViewportOrientation(Ogre::OR_LANDSCAPELEFT),
    mTargetNode(NULL),
    mTightness(0.9)
{
    mCamera = sceneManager->createCamera("OGKCamera");
    mCamera->setFixedYawAxis(true);
    
    mViewport = renderWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.8f, 0.7f, 0.6f, 1.0f));

	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
	mCamera->setNearClipDistance(1);
    mCamera->setFarClipDistance(11000);
    
	mViewport->setCamera(mCamera);
    
    mCameraNode = sceneManager->getRootSceneNode()->createChildSceneNode("OGKCameraNode");
    mCameraNode->attachObject(mCamera);
    mCameraNode->setFixedYawAxis(true);
    
//    mTargetNode = sceneManager->getRootSceneNode()->createChildSceneNode("OGKCameraNodeTarget");
    
    OGKInputManager::getSingletonPtr()->addKeyListener(this, "OGKCameraListener");
    
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->addMultiTouchListener(this, "OGKCameraListener");
#else
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "OGKCameraListener");
#endif
}

OGKCamera::~OGKCamera()
{
    mCameraNode->detachAllObjects();
    delete mCamera;
    Ogre::Root *root = Ogre::Root::getSingletonPtr();
    Ogre::SceneManager *mgr = root->_getCurrentSceneManager();
    if(mgr) {
        mgr->destroySceneNode("OGKCameraNode");
        mgr->destroySceneNode("OGKCameraNodeTarget");
    }
}

Ogre::Camera *OGKCamera::getCamera()
{
    return mCamera;
}

OGKCamera::CameraMode OGKCamera::getMode()
{
    return mMode;
}

void OGKCamera::setMode(OGKCamera::CameraMode mode)
{
    mMode = mode;
    if(mMode == THIRD_PERSON) {
        // @TODO don't hard code this offset
        mCamera->setPosition(0, 2.f, 10.f);
        if(mTargetNode) {
            mCamera->setAutoTracking(true, mTargetNode);
        }
    }
    else {
        mCamera->setPosition(0, 0, 0);
        mCamera->setAutoTracking(false);
    }
}

Ogre::Real OGKCamera::getMoveSpeed()
{
    return mMoveSpeed;
}

void OGKCamera::setMoveSpeed(Ogre::Real moveSpeed)
{
    mMoveSpeed = moveSpeed;
}

void OGKCamera::setTarget(Ogre::SceneNode *target)
{
    mTargetNode = target;
}

Ogre::Real OGKCamera::getTightness()
{
    return mTightness;
}

void OGKCamera::setTightness(Ogre::Real tightness)
{
    mTightness = tightness;
}

bool OGKCamera::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    return TRUE;
}

bool OGKCamera::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return TRUE;
}

#ifdef OGRE_IS_IOS
bool OGKCamera::touchMoved(const OIS::MultiTouchEvent &evt)
{
    if(mMode == FREE) {
        OIS::MultiTouchState state = evt.state;
        int origTransX = 0, origTransY = 0;
    #if !OGRE_NO_VIEWPORT_ORIENTATIONMODE
        switch(mCamera->getViewport()->getOrientationMode())
    #else
        switch(mViewportOrientation)
    #endif
        {
            case Ogre::OR_LANDSCAPELEFT:
                origTransX = state.X.rel;
                origTransY = state.Y.rel;
                state.X.rel = -origTransY;
                state.Y.rel = origTransX;
                break;

            case Ogre::OR_LANDSCAPERIGHT:
                origTransX = state.X.rel;
                origTransY = state.Y.rel;
                state.X.rel = origTransY;
                state.Y.rel = origTransX;
                break;

                // Portrait doesn't need any change
            case Ogre::OR_PORTRAIT:
            default:
                break;
        }

        mCamera->yaw(Ogre::Degree(state.X.rel * -0.1));
        mCamera->pitch(Ogre::Degree(state.Y.rel * -0.1));
    }
    return TRUE;
}

bool OGKCamera::touchPressed(const OIS::MultiTouchEvent &evt)
{
    return TRUE;
}

bool OGKCamera::touchReleased(const OIS::MultiTouchEvent &evt)
{
    return TRUE;
}

bool OGKCamera::touchCancelled(const OIS::MultiTouchEvent &evt)
{
    return TRUE;
}

#else
bool OGKCamera::mouseMoved(const OIS::MouseEvent &evt)
{
    if(mMode == OGKCamera::FREE) {
        mCameraNode->yaw(Ogre::Degree(evt.state.X.rel * -0.1f));
        mCameraNode->pitch(Ogre::Degree(evt.state.Y.rel * -0.1f));
    }
    return TRUE;    
}
////////////////////////////////////////////////////////////////////////////////
bool OGKCamera::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool OGKCamera::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}
#endif

void OGKCamera::update(Ogre::Real elapsedTime)
{
    if(mMode == OGKCamera::FREE) {
        Ogre::Vector3 translateVector(0,0,0);
        Ogre::Real moveScale = mMoveSpeed   * (float)elapsedTime;
#if !defined(OGRE_IS_IOS)
        OIS::Keyboard *keyboard = OGKInputManager::getSingletonPtr()->getKeyboard();
        
        if(keyboard->isKeyDown(OIS::KC_A))
            translateVector.x = -moveScale;
        
        if(keyboard->isKeyDown(OIS::KC_D))
            translateVector.x = moveScale;
        
        if(keyboard->isKeyDown(OIS::KC_W))
            translateVector.z = -moveScale;
        
        if(keyboard->isKeyDown(OIS::KC_S))
            translateVector.z = moveScale;
        
        if(keyboard->isKeyDown(OIS::KC_LSHIFT)) {
            mCameraNode->translate(translateVector * 0.1f,Ogre::Node::TS_LOCAL);
        }
        else {
            mCameraNode->translate(translateVector,Ogre::Node::TS_LOCAL);
        }
#else
        // @TODO
#endif
    }
    else if(mMode == OGKCamera::THIRD_PERSON) {
        mTargetNode->_update(true,true);
//        mCameraNode->_update(true, true);
        Ogre::Vector3 translateVector = (mTargetNode->getPosition() - mCameraNode->getPosition()) * mTightness;
        mCameraNode->translate(translateVector);
        mCameraNode->_update(true, true);
    }
}