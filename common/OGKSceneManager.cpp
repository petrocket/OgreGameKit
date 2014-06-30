//
//  OGKSceneManager.cpp
//  OgreGameKit
//
//  Created by Alex on 6/24/14.
//
//

#include "OGKSceneManager.h"
#include "OGKScene.h"
#include "OGKGame.h"

OGKSceneManager::OGKSceneManager() :
    mActiveScene(NULL),
    mPreviousScene(NULL),
    mRenderTexture(NULL),
    mTransitionNode(NULL),
    mTransitionTimeRemaining(0),
    mTransitionRect(NULL)
{
    
}

OGKSceneManager::~OGKSceneManager()
{
    
}

void OGKSceneManager::addScene(OGKScene *scene, const Ogre::String name)
{
    mScenes.insert( std::pair<Ogre::String, OGKScene *>(name,scene));
}

OGKScene* OGKSceneManager::getActiveScene()
{
    return mActiveScene;
}

OGKScene *OGKSceneManager::getScene(const Ogre::String name)
{
    std::map<Ogre::String, OGKScene *>::iterator ii = mScenes.find(name);
    
    if(ii != mScenes.end()) {
        return ii->second;
    }
    
    return NULL;
}

void OGKSceneManager::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
    if(mTransitionRect) {
        mTransitionRect->setVisible(false);
    }
    if(mActiveScene) {
        mActiveScene->mSceneNode->setVisible(false);
    }
    if(mPreviousScene) {
        mPreviousScene->mSceneNode->setVisible(true);
    }
}

void OGKSceneManager::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
    if(mTransitionRect) {
        mTransitionRect->setVisible(true);
    }
    if(mActiveScene) {
        mActiveScene->mSceneNode->setVisible(true);
    }
    if(mPreviousScene) {
        mPreviousScene->mSceneNode->setVisible(false);
    }
}

void OGKSceneManager::setActiveScene(const Ogre::String name, Ogre::Real transitionTime)
{
    mPreviousScene = mActiveScene;
    mActiveScene = getScene(name);
    
    Ogre::ColourValue bg = Ogre::ColourValue::Black;
    
    if(mPreviousScene) {
        if(mPreviousScene->mCamera) {
            // save viewport settings for rtt
            Ogre::Camera *cam = mPreviousScene->mCamera->getCamera();
            bg = cam->getViewport()->getBackgroundColour();
        }
        mPreviousScene->onExitTransitionDidStart();
    }
    
    if(mActiveScene) {
        OGKGame::getSingletonPtr()->mLog->logMessage("Transitioning to " + name + " scene");
        mActiveScene->onEnter();
    }
    
    if(transitionTime > 0.01) {
        mTransitionTime = transitionTime;
        mTransitionTimeRemaining = transitionTime;
        
        _initRTT();
        
        if(mPreviousScene && mPreviousScene->mCamera &&
           mPreviousScene->mCamera->getCamera()) {
            // render to texture instead
            Ogre::Camera *cam = mPreviousScene->mCamera->getCamera();
            mRenderTexture->removeAllViewports();
            Ogre::Viewport *vp = mRenderTexture->addViewport(cam);
            if(vp) {
                vp->setClearEveryFrame(true);
                vp->setBackgroundColour(bg);
                vp->setCamera(cam);
            }
        }
        
        // Scene node
        if(mActiveScene && mActiveScene->mSceneManager) {
            mTransitionNode = mActiveScene->mSceneManager->getRootSceneNode()->createChildSceneNode("OGKTransitionNode");
            mTransitionNode->attachObject(mTransitionRect);
        }        
        
        mTransitionRect->setVisible(true);
    }
    else {
        if(mTransitionNode) {
            mTransitionNode->detachAllObjects();
        }
        
        if(mPreviousScene) mPreviousScene->onExit();
        if(mActiveScene) mActiveScene->onEnterTransitionDidFinish();
    }
}

void OGKSceneManager::update(Ogre::Real timeElapsed)
{
    if(mTransitionTimeRemaining > 0.f) {
        mTransitionTimeRemaining -= timeElapsed;
        if(mTransitionTimeRemaining <= 0.f) {
            OGKGame::getSingletonPtr()->mLog->logMessage("Transition completed");
            if(mPreviousScene) {
                mPreviousScene->onExit();
            }

            if(mActiveScene) {
                mActiveScene->onEnterTransitionDidFinish();
            }
            
            if(mTransitionNode) {
                mTransitionNode->detachAllObjects();
            }
            
            if(mTransitionRect) {
                mTransitionRect->setVisible(false);
            }
        }
        else if(mPreviousScene) {
            mPreviousScene->update(timeElapsed);
            
            if(mRenderTexture) {
                mRenderTexture->update();
            }
            
            if(mTransitionRect) {
                Ogre::MaterialPtr mat = mTransitionRect->getMaterial();
                Ogre::Real fadeAmt = MIN(1.0,MAX(0.0,mTransitionTimeRemaining) / mTransitionTime);
                Ogre::TextureUnitState *tex = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
                tex->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, fadeAmt);
            }
        }
    }
    
    if(mActiveScene) {
        mActiveScene->update(timeElapsed);
    }
}

void OGKSceneManager::_initRTT()
{
    if(mTransitionTexture.isNull()) {
        Ogre::TextureManager *mgr = Ogre::TextureManager::getSingletonPtr();
        mTransitionTexture = mgr->createManual("OGKTransitionTexture",
                                               Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                               Ogre::TEX_TYPE_2D,
                                               OGKGame::getSingletonPtr()->mRenderWindow->getWidth(),
                                               OGKGame::getSingletonPtr()->mRenderWindow->getHeight(),
                                               0,
                                               Ogre::PF_R8G8B8A8,
                                               Ogre::TU_RENDERTARGET);
        
    }
    if(!mRenderTexture) {
        mRenderTexture = mTransitionTexture->getBuffer()->getRenderTarget();
        mRenderTexture->setAutoUpdated(false);
        mRenderTexture->addListener(this);
    }
    
    if(!mTransitionRect) {
        // Rect
        mTransitionRect = OGRE_NEW Ogre::Rectangle2D(true);
        mTransitionRect->setCorners(-1.0,1.0,1,-1);
        mTransitionRect->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
        mTransitionRect->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY + 1);
        
        // Material
        Ogre::MaterialManager *mgr = Ogre::MaterialManager::getSingletonPtr();
        Ogre::MaterialPtr mat = mgr->create("OGKTransitionMaterial",
                                            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        mat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
        mat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        mat->getTechnique(0)->getPass(0)->createTextureUnitState("OGKTransitionTexture");
        mTransitionRect->setMaterial("OGKTransitionMaterial");
    }
}