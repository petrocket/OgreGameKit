//
//  OGKSceneManager.h
//  OgreGameKit
//
//  Created by Alex on 6/24/14.
//
//

#ifndef __OgreGameKit__OGKSceneManager__
#define __OgreGameKit__OGKSceneManager__

#include <Ogre.h>
#include <OgreOverlaySystem.h>

class OGKScene;

class OGKSceneManager : public Ogre::RenderTargetListener
{
public:
    OGKSceneManager();
    ~OGKSceneManager();
    
    void addScene(OGKScene *scene, const Ogre::String name);
    
    OGKScene *getActiveScene();

    OGKScene *getScene(const Ogre::String name);
    
    void setActiveScene(const Ogre::String name, Ogre::Real transitionTime = 0.0);
    
    void update(Ogre::Real timeElapsed);
    
protected:
    virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    
private:
    void _initRTT();
    
    // scenes
    OGKScene *mActiveScene;
    OGKScene *mPreviousScene;
    
    std::map<Ogre::String, OGKScene *> mScenes;

    // transition elements
    Ogre::Overlay *mOverlay;
    Ogre::OverlayContainer *mPanel;
    Ogre::RenderTexture *mRenderTexture;
    Ogre::TexturePtr mTransitionTexture;
    Ogre::MaterialPtr mTransitionMaterial;
    Ogre::Real mTransitionTime;
    Ogre::Real mTransitionTimeRemaining;
    Ogre::TextureUnitState *mTransitionTextureUnitState;
};

#endif /* defined(__OgreGameKit__OGKSceneManager__) */
