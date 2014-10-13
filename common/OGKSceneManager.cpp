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

#ifdef INCLUDE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"
#endif

const Ogre::String kTransitionTextureName = "OGKTransitionTexture";
const Ogre::String kTransitionMaterialName = "OGKTransitionMaterial";
const Ogre::String kTransitionNodeName = "OGKTransitionNode";
const Ogre::String kTransitionOverlayName = "OGKTransitionOverlay";
const Ogre::String kTransitionOverlayPanelName = "OGKTransitionOverlayPanel";

OGKSceneManager::OGKSceneManager() :
    mActiveScene(NULL),
    mPanel(NULL),
    mPreviousScene(NULL),
    mRenderTexture(NULL),
    mTransitionTimeRemaining(0),
    mTransitionTextureUnitState(NULL)
{
    mOverlay = Ogre::OverlayManager::getSingletonPtr()->create(kTransitionOverlayName);
    mOverlay->setZOrder(1000);
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
    if(mOverlay) mOverlay->hide();
    
    if(mActiveScene) {
        mActiveScene->mSceneNode->setVisible(false);
        if(mActiveScene->mOverlay) {
            mActiveScene->mOverlay->hide();
        }
    }
    if(mPreviousScene) {
        if(mPreviousScene->mSceneNode) {
            mPreviousScene->mSceneNode->setVisible(true);
        }
        if(mPreviousScene->mOverlay) {
            mPreviousScene->mOverlay->show();
        }
    }
}

void OGKSceneManager::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
    if(mOverlay) mOverlay->show();
    
    if(mActiveScene) {
        mActiveScene->mSceneNode->setVisible(true);
        if(mActiveScene->mOverlay) {
            mActiveScene->mOverlay->show();
        }
    }
    if(mPreviousScene) {
        mPreviousScene->mSceneNode->setVisible(false);
        if(mPreviousScene->mOverlay) {
            mPreviousScene->mOverlay->hide();
        }
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
        OGKLOG("Transitioning to " + name + " scene");
        mActiveScene->onEnter();
    }
    
    if(transitionTime > 0.01) {
        // transition
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
                //OGKLOG("Preparing rtt viewport");
                vp->setClearEveryFrame(true);
                vp->setBackgroundColour(bg);
                vp->setOverlaysEnabled(true);
                vp->setCamera(cam);
            }
        }

        if(mOverlay) {
            //OGKLOG("Showing Overlay");
            mOverlay->show();
        }
    }
    else {
        if(mPreviousScene) mPreviousScene->onExit();
        if(mActiveScene) mActiveScene->onEnterTransitionDidFinish();
    }
}

void OGKSceneManager::update(Ogre::Real timeElapsed)
{
    if(mTransitionTimeRemaining > 0.f) {
        mTransitionTimeRemaining -= timeElapsed;
        if(mTransitionTimeRemaining <= 0.f) {
            OGKLOG("Transition completed");
            if(mPreviousScene) mPreviousScene->onExit();
            if(mActiveScene) mActiveScene->onEnterTransitionDidFinish();
            if(mOverlay) mOverlay->hide();
        }
        else if(mPreviousScene) {
            mPreviousScene->update(timeElapsed);
            
            if(mRenderTexture) {
                //OGKLOG("updating render texture");
                mRenderTexture->update();
            }
            
            if(mTransitionTextureUnitState) {
                Ogre::Real fadeAmt = MIN(1.0,MAX(0.0,mTransitionTimeRemaining) / mTransitionTime);
                //OGKLOG("updating fade amt " +  Ogre::StringConverter::toString(fadeAmt) );
                
#ifdef OGRE_IS_IOS
                // Retrieve the shader parameters
                Ogre::GpuProgramParametersSharedPtr pParams = mTransitionMaterial->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
                if ( !pParams.isNull() ) {
                    if ( pParams->_findNamedConstantDefinition( "quadAlpha" ) ) {
                        pParams->setNamedConstant( "quadAlpha", fadeAmt );
                    }
                }
#else
                mTransitionTextureUnitState->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, fadeAmt);
#endif
                
#ifdef INCLUDE_RTSHADER_SYSTEM
                Ogre::RTShader::ShaderGenerator::getSingletonPtr()->invalidateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, kTransitionMaterialName);
#endif
            }
        }
    }
    
    if(mActiveScene) mActiveScene->update(timeElapsed);
}

void OGKSceneManager::_initRTT()
{
    if(mTransitionTexture.isNull()) {
        Ogre::TextureManager *mgr = Ogre::TextureManager::getSingletonPtr();
        
        mTransitionTexture = mgr->createManual(kTransitionTextureName,
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
    
    if(!mPanel) {
        Ogre::MaterialManager *mgr = Ogre::MaterialManager::getSingletonPtr();
        
#ifdef OGRE_IS_IOS
        mTransitionMaterial = mgr->getByName("OGK/SceneTransitionMaterial");
        
        //mTransitionMaterial->getTechnique(0)->getPass(0)->removeAllTextureUnitStates();
        //Ogre::TextureUnitState* state = mTransitionMaterial->getTechnique(0)->getPass(0)->createTextureUnitState();
#else
        mTransitionMaterial = mgr->create(kTransitionMaterialName,
                                          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        mTransitionMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
        mTransitionMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        mTransitionMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(kTransitionTextureName);
        
#endif
        mTransitionTextureUnitState = mTransitionMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);
        
#ifdef OGRE_IS_IOS
//        mTransitionTextureUnitState->setTextureFiltering( Ogre::FO_ANISOTROPIC, Ogre::FO_ANISOTROPIC, Ogre::FO_LINEAR );
//        mTransitionTextureUnitState->setTextureAnisotropy( 8 );
//        mTransitionTextureUnitState->setTextureName( kTransitionTextureName );
        mTransitionTextureUnitState->setTexture(mTransitionTexture);
#endif
        
        mPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement( "Panel", kTransitionOverlayPanelName));
        mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
        mPanel->setPosition(0.0, 0.0);
        mPanel->setDimensions(1.0, 1.0);
#ifdef OGRE_IS_IOS
        mPanel->setMaterialName( "OGK/SceneTransitionMaterial" );
#else
        mPanel->setMaterialName( kTransitionMaterialName );
#endif
//        mPanel->setMaterialName( "OGK/Debug/Yellow" );
//        mPanel->setMaterialName( "BaseWhiteNoLighting" );
        mOverlay->add2D(mPanel);
        mOverlay->show();
    }
}