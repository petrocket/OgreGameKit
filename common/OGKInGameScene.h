//
//  OGKInGameScene.h
//  OgreGameKit
//
//  Created by Alex on 6/24/14.
//
//

#ifndef __OgreGameKit__OGKInGameScene__
#define __OgreGameKit__OGKInGameScene__

#include "OGKScene.h"

#include "OGKCamera.h"

// player
#include "OGKPlayer.h"

// npc
#include "OGKNPC.h"

// terrain
#include "OGKTerrain.h"

#include "Gui3D.h"
#include "Gui3DButton.h"
#include "Gui3DScreenRenderable2DPanel.h"

class OGKInGameScene : public OGKScene
{
public:
    OGKInGameScene(const Ogre::String& name);
    ~OGKInGameScene();
    
    void init();
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    void onExitTransitionDidStart();
    
    inline Gui3D::Gui3D *getGUI() { return mGUI; }
    inline OGKTerrain *getTerrain() { return mTerrain; }
    inline OGKPlayer *getPlayer() { return mPlayer; }
    
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
    
private:
    bool buttonPressed(Gui3D::PanelElement *e);
    void interactWithNPC(OGKNPC *npc);
    OGKNPC *getNPC(Ogre::Entity *entity);
    
    void _initLoadingPanel();
    void _initMenuPanel();
    void _initHUDPanel();
    void _initDialogPanel();
    
    void _handleClickEvent(Ogre::Real x, Ogre::Real y);
    
    void _updateHUD();
    
    // player
    OGKPlayer *mPlayer;
    Ogre::Real mLastHealth;
    
    std::vector<OGKNPC*> mNPCs;
    
    // terrain
    OGKTerrain *mTerrain;
    Ogre::Light *mLight;
    
    // gui
    Gui3D::Gui3D *mGUI;
    
    Gui3D::ScreenRenderable2DPanel *mLoadingPanel;
    Gui3D::Caption *mLoadingCaption;
    
    Gui3D::ScreenRenderable2DPanel *mMenuPanel;
    Gui3D::Button *mMenuButton;
    Gui3D::Button *mResumeButton;

    Gui3D::ScreenRenderable2DPanel *mHUDPanel;
#if OGRE_IS_IOS
    Gui3D::Button *mCamModeButton;
#endif
    Gui3D::Button *mPauseButton;
    Gorilla::Rectangle *mHeart1;
    Gorilla::Rectangle *mHeart2;
    Gorilla::Rectangle *mHeart3;
    
    Gui3D::ScreenRenderable2DPanel *mDialogPanel;
    Gui3D::Caption *mDialogCaption;
    Gui3D::Button *mCloseDialogButton;
};

#endif /* defined(__OgreGameKit__OGKInGameScene__) */
