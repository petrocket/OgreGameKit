//
//  OGKMenuScene.h
//  OgreGameKit
//
//  Created by Alex on 6/24/14.
//
//

#ifndef __OgreGameKit__OGKMenuScene__
#define __OgreGameKit__OGKMenuScene__

#include "OGKScene.h"

#include "OGKCamera.h"

#include "Gui3D.h"
#include "Gui3DCaption.h"
#include "Gui3DButton.h"
#include "Gui3DCheckboxText.h"
#include "Gui3DCombobox.h"
#include "Gui3DScreenPanel.h"
#include "Gui3DScreenRenderable2DPanel.h"

class OGKMenuScene : public OGKScene
{
public:
    OGKMenuScene(const Ogre::String& name);
    ~OGKMenuScene();
    void init();
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    void onExitTransitionDidStart();
    
    void update(Ogre::Real elapsedTime);
    
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
    bool stateChanged(Gui3D::PanelElement *e);

    void _createMainMenu();
    void _createLogo();
    void _createSettingsMenu();

    Gui3D::Gui3D *mGUI;
    
    // Main Menu
    Gui3D::ScreenRenderable2DPanel *mMainPanel;
    Gui3D::Button *mPlayButton;
    Gui3D::Button *mQuitButton;
    Gui3D::Button *mSettingsButton;
    
    // Settings
    Gui3D::ScreenRenderable2DPanel *mSettingsPanel;
    Gui3D::Caption *mSettingsCaption;
    Gui3D::Button *mBackButton;
    Gui3D::Caption *mFullScreenCaption;
    Gui3D::CheckboxText *mFullScreenCheckbox;
    Gui3D::Caption *mResolutionCaption;
    Gui3D::Combobox *mResolutionCombobox;
};

#endif /* defined(__OgreGameKit__OGKMenuScene__) */
