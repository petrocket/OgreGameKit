//
//  SinglePlayerScene.h
//  OgreGameKit
//
//  Created by Alex on 10/9/14.
//
//

#ifndef __OgreGameKit__SinglePlayerScene__
#define __OgreGameKit__SinglePlayerScene__

// Ogre Game Kit
#include "OGKScene.h"
#include "OGKCamera.h"
#include "OGKPlayer.h"
#include "OGKNPC.h"

// Gui3D
#include "Gui3D.h"
#include "Gui3DButton.h"
#include "Gui3DScreenRenderable2DPanel.h"

// Collision
#include "OgreRayCollision.h"

class SinglePlayerScene : public OGKScene
{
public:
    SinglePlayerScene(const Ogre::String& name);
    ~SinglePlayerScene();
    
    void init();
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    void onExitTransitionDidStart();
    
    inline Gui3D::Gui3D *getGUI() { return mGUI; }
    inline OGKPlayer *getPlayer() { return mPlayer; }
    OGKNPC *getNPC(Ogre::Entity *entity);
    
    virtual void update(Ogre::Real elapsedTime);
    
    // input
    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    
#ifdef OGRE_IS_IOS
    bool touchMoved(const OIS::MultiTouchEvent &evt);
    bool touchPressed(const OIS::MultiTouchEvent &evt);
    bool touchReleased(const OIS::MultiTouchEvent &evt);
    bool touchCancelled(const OIS::MultiTouchEvent &evt);
#else
    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
#endif
    
protected:
    Ogre::MovableObject *getTerrainObject();

private:
    bool buttonPressed(Gui3D::PanelElement *e);
    void interactWithNPC(OGKNPC *npc);

    void _handleClickEvent(Ogre::Real x, Ogre::Real y);

    void _initGUI();
    void _initLoadingPanel();
    void _initMenuPanel();
    void _initHUDPanel();
    void _initDialogPanel();

    void _loadLevel();
    void _unloadLevel();
    
    void _updateHUD();
    
    OGKPlayer *mPlayer;
    Ogre::Real mLastHealth;    
    std::vector<OGKNPC*> mNPCs;
    
    Ogre::Light *mLight;
    Ogre::MovableObject *mTerrainObject;
    
    // Gui3D
    Gui3D::Gui3D *mGUI;
    
    // Loading
    Gui3D::ScreenRenderable2DPanel *mLoadingPanel;
    Gui3D::Caption *mLoadingCaption;
    
    // Pause menu
    Gui3D::ScreenRenderable2DPanel *mMenuPanel;
    Gui3D::Button *mMenuButton;
    Gui3D::Button *mResumeButton;
    
    // HUD
    Gui3D::ScreenRenderable2DPanel *mHUDPanel;
#if OGRE_IS_IOS
    Gui3D::Button *mCamModeButton;
#endif
    Gui3D::Button *mPauseButton;
    Gorilla::Rectangle *mHeart1;
    Gorilla::Rectangle *mHeart2;
    Gorilla::Rectangle *mHeart3;
    
    // Dialog (for NPCs)
    Gui3D::ScreenRenderable2DPanel *mDialogPanel;
    Gui3D::Caption *mDialogCaption;
    Gui3D::Button *mCloseDialogButton;
};

#endif /* defined(__OgreGameKit__SinglePlayerScene__) */
