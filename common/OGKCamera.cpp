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
    mMode(FREE),
    mMoveSpeed(1.0),
    mViewportOrientation(Ogre::OR_LANDSCAPELEFT)
{
    mCamera = sceneManager->createCamera("OGKCamera");
    
    mViewport = renderWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.8f, 0.7f, 0.6f, 1.0f));

	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
	mCamera->setNearClipDistance(1);
    mCamera->setFarClipDistance(11000);
    
	mViewport->setCamera(mCamera);
    
    mCameraNode = sceneManager->getRootSceneNode()->createChildSceneNode("OGKCameraNode");
    mTargetNode = sceneManager->getRootSceneNode()->createChildSceneNode("OGKCameraNodeTarget");
    
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
}

Ogre::Real OGKCamera::getMoveSpeed()
{
    return mMoveSpeed;
}

void OGKCamera::setMoveSpeed(Ogre::Real moveSpeed)
{
    mMoveSpeed = moveSpeed;
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
        mCamera->yaw(Ogre::Degree(evt.state.X.rel * -0.1f));
        mCamera->pitch(Ogre::Degree(evt.state.Y.rel * -0.1f));
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
            mCamera->moveRelative(translateVector * 0.1f);
        }
        else {
            mCamera->moveRelative(translateVector);
        }
#else
        // @TODO
#endif
    }
}