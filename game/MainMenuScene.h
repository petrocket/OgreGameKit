//
//  MainMenuScene.h
//  OgreGameKit
//
//  Created by Alex on 10/9/14.
//
//

#ifndef __OgreGameKit__MainMenuScene__
#define __OgreGameKit__MainMenuScene__

// Ogre Game Kit
#include "OGKScene.h"
#include "OGKCamera.h"

// Gui3D
#include "Gui3D.h"
#include "Gui3DCaption.h"
#include "Gui3DButton.h"
#include "Gui3DCheckboxText.h"
#include "Gui3DCombobox.h"
#include "Gui3DScreenRenderable2DPanel.h"

class MainMenuScene : public OGKScene
{
public:
    MainMenuScene(const Ogre::String& name);
    ~MainMenuScene();
    
    void init();
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    void onExitTransitionDidStart();
    
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
    // input handlers
    bool buttonPressed(Gui3D::PanelElement *e);
    bool stateChanged(Gui3D::PanelElement *e);
    
    // create menus & gui elements
    void _createMainMenu();
    void _createLogo();
    void _createSettingsMenu();
    
    Gui3D::Gui3D *mGUI;
    
    // Main Menu
    Gui3D::ScreenRenderable2DPanel *mMainPanel;
    Gui3D::Button *mPlayButton;
    Gui3D::Button *mQuitButton;
    Gui3D::Button *mSettingsButton;
    
    // Settings Menu
    Gui3D::ScreenRenderable2DPanel *mSettingsPanel;
    Gui3D::Caption *mSettingsCaption;
    Gui3D::Button *mBackButton;
    Gui3D::Caption *mFullScreenCaption;
    Gui3D::CheckboxText *mFullScreenCheckbox;
    Gui3D::Caption *mResolutionCaption;
    Gui3D::Combobox *mResolutionCombobox;
};
#endif /* defined(__OgreGameKit__MainMenuScene__) */
