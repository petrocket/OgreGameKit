//
//  OGKCamera.cpp
//  OgreGameKit
//
//  Created by Alex on 6/19/14.
//
//

#include "OGKCamera.h"
#include "OGKInputManager.h"
#include "OGKGame.h"

OGKCamera::OGKCamera(
                     Ogre::SceneManager *sceneManager,
                     Ogre::RenderWindow *renderWindow) :
    mCamera(NULL),
    mEdgeBuffer(1.0),
    mLookAtOffset(0,0,-10.0),
    mMode(FREE),
    mMoveSpeed(1.0),
    mViewportOrientation(Ogre::OR_LANDSCAPELEFT),
    mTargetNode(NULL),
    mTargetOffset(0,5.0,10.5),
    mTightness(0.97)
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
    
    loadFromConfig();
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

void OGKCamera::loadFromConfig()
{
    Ogre::ConfigFile *cf = OGKGame::getSingletonPtr()->getGameConfig();
    if(cf) {
        Ogre::String setting;
        setting = cf->getSetting("edgeBuffer","camera");
        if(setting.length()) {
            mEdgeBuffer = Ogre::StringConverter::parseReal(setting);
        }
        setting = cf->getSetting("moveSpeed","camera");
        if(setting.length()) {
            setMoveSpeed(Ogre::StringConverter::parseReal(setting));
        }
        setting = cf->getSetting("tightness","camera");
        if(setting.length()) {
            setTightness(Ogre::StringConverter::parseReal(setting));
        }
        setting = cf->getSetting("targetOffset","camera");
        if(setting.length()) {
            mTargetOffset = Ogre::StringConverter::parseVector3(setting);
        }
        setting = cf->getSetting("lookAtOffset","camera");
        if(setting.length()) {
            mLookAtOffset = Ogre::StringConverter::parseVector3(setting);
        }
        setting = cf->getSetting("mode","camera");
        if(setting.length()) {
            if(setting.compare("first person") == 0) {
                setMode(OGKCamera::FIRST_PERSON);
            }
            else if(setting.compare("third person") == 0) {
                setMode(OGKCamera::THIRD_PERSON);
            }
            else if(setting.compare("third person indirect") == 0) {
                setMode(OGKCamera::THIRD_PERSON_INDIRECT);
            }
            else if(setting.compare("free") == 0) {
                setMode(OGKCamera::FREE);
            }
        }
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
    
    Ogre::Vector3 oldDirection = mCamera->getOrientation() * -Ogre::Vector3::UNIT_Z;
    
    mCamera->setAutoTracking(false);
    mCamera->setOrientation(Ogre::Quaternion::IDENTITY);
    mCameraNode->setFixedYawAxis(true);
    
    if(mMode == THIRD_PERSON || mMode == THIRD_PERSON_INDIRECT) {
        if(mTargetNode) {
            mCamera->setAutoTracking(true, mTargetNode, mLookAtOffset);
        }
    }
    else if(mMode == FIXED) {
        if(mTargetNode) {
            mCameraNode->lookAt(mTargetNode->getPosition(), Ogre::Node::TS_LOCAL);
        }
    }
    else if(mMode == FREE) {
        // maintain the old look at direction
        mCameraNode->lookAt(oldDirection, Ogre::Node::TS_LOCAL);
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
        mCameraNode->yaw(Ogre::Degree(evt.state.X.rel * -0.1f),Ogre::SceneNode::TS_WORLD);
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
    else if(mMode == OGKCamera::FIRST_PERSON) {
        mTargetNode->_update(true,true);
        mCameraNode->setPosition(mTargetNode->getPosition() + mTargetOffset);
        mCameraNode->setOrientation(mTargetNode->getOrientation());
    }
    else if(mMode == OGKCamera::THIRD_PERSON) {
        mTargetNode->_update(true,true);
        Ogre::Vector3 translateVector = ((mTargetNode->getPosition() + (mTargetNode->getOrientation() * mTargetOffset)) - mCameraNode->getPosition()) * mTightness;
        mCameraNode->translate(translateVector);
    }
    else if(mMode == OGKCamera::THIRD_PERSON_INDIRECT) {
        mTargetNode->_update(true,true);
        
        if(mEdgeBuffer < 1.0) {
            // get the target's screen position
            Ogre::Vector3 coords = mCamera->getProjectionMatrix() * mCamera->getViewMatrix() * mTargetNode->getPosition();
            Ogre::Ray ray = mCamera->getCameraToViewportRay(0, 0);
            
            // @TODO cal distance from ray to target node position
        }
        
        Ogre::Vector3 translateVector = ((mTargetNode->getPosition() + mTargetOffset) - mCameraNode->getPosition()) * mTightness;
        mCameraNode->translate(translateVector);
    }
}