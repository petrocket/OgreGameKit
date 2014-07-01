//
//  OGKMenuScene.cpp
//  OgreGameKit
//
//  Created by Alex on 6/24/14.
//
//

#include "OGKMenuScene.h"
#include "OGKGame.h"

// DEFAULT THEME
class Gui3DTheme : public Gui3D::PanelColors
{
public:
    inline Gui3DTheme():Gui3D::PanelColors()
    {
        transparent = Gorilla::rgb(0,0,0,0);
        
        // Panel
        panelBackgroundSpriteName = "panelBackground";
        panelCursorSpriteName = "mousepointer";
        panelCursorSpriteSizeX = 15;
        panelCursorSpriteSizeY = 22;
        
        // Button
//        buttonInactiveSpriteName = "buttonInactive";
//        buttonOveredSpriteName = "buttonOvered";
//        buttonNotOveredSpriteName = "buttonNotOvered";
//        buttonClickedSpriteName = "buttonClicked";
        
        buttonBackgroundClickedGradientType = Gorilla::Gradient_NorthSouth;
        buttonBackgroundClickedGradientStart = Gorilla::rgb(255, 255, 255, 200);
        buttonBackgroundClickedGradientEnd = Gorilla::rgb(255, 255, 255, 170);
        
        buttonBackgroundOveredGradientType = Gorilla::Gradient_NorthSouth;
        buttonBackgroundOveredGradientStart = Gorilla::rgb(255, 255, 255, 128);
        buttonBackgroundOveredGradientEnd = Gorilla::rgb(255, 255, 255, 100);
        
        buttonBackgroundNotOveredGradientType = Gorilla::Gradient_NorthSouth;
        buttonBackgroundNotOveredGradientStart = Gorilla::rgb(255, 255, 255, 80);
        buttonBackgroundNotOveredGradientEnd = Gorilla::rgb(255, 255, 255, 50);
        
        buttonBackgroundInactiveGradientType = Gorilla::Gradient_NorthSouth;
        buttonBackgroundInactiveGradientStart = Gorilla::rgb(255, 255, 255, 15);
        buttonBackgroundInactiveGradientEnd = Gorilla::rgb(255, 255, 255, 5);
        
        buttonText = Gorilla::rgb(255, 255, 255, 255);
        buttonTextInactive = Gorilla::rgb(255, 255, 255, 70);
        buttonTextSize = 14;
        
        // @TODO add other button styles
        
        // Caption
        captionBackgroundGradientType = Gorilla::Gradient_NorthSouth;
        captionBackgroundGradientStart = transparent;
        captionBackgroundGradientEnd = transparent;
        
        captionBorder = transparent;
        captionText = Ogre::ColourValue::White;
        captionTextSize = 14;
        captionBorderSize = 0;
        
        // Checkbox
        checkboxTextSize = 14;
        checkboxCheckedSymbol = 'x';
        checkboxBorderSize = 0;
        
        checkboxSelectedOveredGradientType = Gorilla::Gradient_NorthSouth;
        checkboxSelectedOveredGradientStart = Gorilla::rgb(255, 255, 255, 200);
        checkboxSelectedOveredGradientEnd = Gorilla::rgb(255, 255, 255, 170);
        
        checkboxOveredGradientType = Gorilla::Gradient_NorthSouth;
        checkboxOveredGradientStart = Gorilla::rgb(255, 255, 255, 128);
        checkboxOveredGradientEnd = Gorilla::rgb(255, 255, 255, 100);
        
        checkboxSelectedNotOveredGradientType = Gorilla::Gradient_NorthSouth;
        checkboxSelectedNotOveredGradientStart = Gorilla::rgb(255, 255, 255, 80);
        checkboxSelectedNotOveredGradientEnd = Gorilla::rgb(255, 255, 255, 50);
        
        checkboxNotOveredGradientType = Gorilla::Gradient_NorthSouth;
        checkboxNotOveredGradientStart = Gorilla::rgb(255, 255, 255, 15);
        checkboxNotOveredGradientEnd = Gorilla::rgb(255, 255, 255, 5);
    }
};

static Gui3DTheme gui3DTheme;

OGKMenuScene::OGKMenuScene(const Ogre::String& name):OGKScene(name),
    mGUI(NULL),
    mMainPanel(NULL),
    mSettingsPanel(NULL)
{
    init();
}

OGKMenuScene::~OGKMenuScene()
{
    
}

bool OGKMenuScene::buttonPressed(Gui3D::PanelElement *e)
{
    if(e == mQuitButton) {
        OGKGame::getSingletonPtr()->shutdown();
    }
    else if(e == mBackButton) {
        mMainPanel->setVisible(true);
        mSettingsPanel->setVisible(false);
    }
    else if(e == mPlayButton) {
        OGKGame::getSingletonPtr()->mGameSceneManager->setActiveScene("ingame", 2000);
    }
    else if(e == mSettingsButton) {
        mMainPanel->setVisible(false);
        mSettingsPanel->setVisible(true);
        
    }
    else {
        return true;
    }
    
    return false;
}

void OGKMenuScene::init()
{
    // don't call OGKScene init (it's constructor already does that)
    mGUI = OGRE_NEW Gui3D::Gui3D(&gui3DTheme);
}

void OGKMenuScene::onEnter()
{
    OGKScene::onEnter();

    // set the camera position
    mCamera->getCamera()->setPosition(0, 0, 0);
    mCamera->getCamera()->lookAt(0,0,10);
    
    // create the gui
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    mScreen = mGUI->createScreenRenderable2D(vp, "default_theme","menu");
    Ogre::SceneNode *node = OGRE_NEW Ogre::SceneNode(mSceneManager);
    node->attachObject(mScreen);
    mOverlay->add3D(node);
    mOverlay->show();

    _createMainMenu();
    _createSettingsMenu();
    _createLogo();

    OGKInputManager::getSingletonPtr()->addKeyListener(this, "menuScene");
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "menuScene");
}

void OGKMenuScene::onEnterTransitionDidFinish()
{
    OGKScene::onEnterTransitionDidFinish();
    if(mMainPanel) mMainPanel->showInternalMousePointer();
}

void OGKMenuScene::onExit()
{
    OGKScene::onExit();
    
    if(mGUI) mGUI->destroyScreenRenderable2D("menu");
    mScreen = NULL;
}

void OGKMenuScene::onExitTransitionDidStart()
{
    OGKScene::onExitTransitionDidStart();
    
    if(mMainPanel) mMainPanel->hideInternalMousePointer();
    
    OGKInputManager::getSingletonPtr()->removeKeyListener("menuScene");
    OGKInputManager::getSingletonPtr()->removeMouseListener("menuScene");
}

bool OGKMenuScene::stateChanged(Gui3D::PanelElement *e)
{
    if(e == mFullScreenCheckbox) {
        Ogre::Real width = OGKGame::getSingletonPtr()->mRenderWindow->getWidth();
        Ogre::Real height = OGKGame::getSingletonPtr()->mRenderWindow->getHeight();
        if(mFullScreenCheckbox->getChecked() &&
           !OGKGame::getSingletonPtr()->mRenderWindow->isFullScreen()) {
            OGKGame::getSingletonPtr()->mRenderWindow->setFullscreen(true, width, height);
            OGKInputManager::getSingletonPtr()->setMouseVisible(false);
        }
        else if(!mFullScreenCheckbox->getChecked() &&
           OGKGame::getSingletonPtr()->mRenderWindow->isFullScreen()) {
            OGKGame::getSingletonPtr()->mRenderWindow->setFullscreen(false, width, height);
            OGKInputManager::getSingletonPtr()->setMouseVisible(false);
        }
    }
}

void OGKMenuScene::update(Ogre::Real elapsedTime)
{
    OGKScene::update(elapsedTime);
}

bool OGKMenuScene::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    switch (keyEventRef.key) {
        case OIS::KC_ESCAPE:
            OGKGame::getSingletonPtr()->shutdown();
            break;
        default:
            break;
    }
    
    return true;
}

#ifdef OGRE_IS_IOS
bool OGKMenuScene::touchMoved(const OIS::MultiTouchEvent &evt) { return true; }
bool OGKMenuScene::touchPressed(const OIS::MultiTouchEvent &evt)  { return true; }
bool OGKMenuScene::touchReleased(const OIS::MultiTouchEvent &evt)  { return true; }
bool OGKMenuScene::touchCancelled(const OIS::MultiTouchEvent &evt)  { return true; }
#else
bool OGKMenuScene::mouseMoved(const OIS::MouseEvent &evt)
{
    if(mMainPanel) {
        mMainPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);
    }
    
    if(mSettingsPanel) {
        mSettingsPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);
    }
    return false;
}

bool OGKMenuScene::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mMainPanel && mMainPanel->isVisible()) {
        mMainPanel->injectMousePressed(evt, id);
    }
    else if(mSettingsPanel && mSettingsPanel->isVisible()) {
        mSettingsPanel->injectMousePressed(evt, id);
    }
    return false;
}
bool OGKMenuScene::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mMainPanel && mMainPanel->isVisible()) {
        mMainPanel->injectMouseReleased(evt, id);
    }
    else if(mSettingsPanel && mSettingsPanel->isVisible()) {
        mSettingsPanel->injectMouseReleased(evt, id);
    }
    return false;
}
#endif

#pragma mark - Private

void OGKMenuScene::_createMainMenu()
{
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    mMainPanel = OGRE_NEW Gui3D::ScreenRenderable2DPanel(mGUI,
                                                     mScreen,
                                                     Ogre::Vector2(0,0),
                                                     Ogre::Vector2(vp->getActualWidth(),
                                                                   vp->getActualHeight()),
                                                     "default_theme",
                                                     "MainMenu");
    mMainPanel->hideInternalMousePointer();
    
    mPlayButton = mMainPanel->makeButton(vp->getActualWidth() / 2 - 100,
                                         vp->getActualHeight() / 2 - 20,
                                         200, 40, "PLAY");
    mPlayButton->setPressedCallback(this, &OGKMenuScene::buttonPressed);

    mQuitButton = mMainPanel->makeButton(5, 5, 120, 30, "QUIT");
    mQuitButton->setPressedCallback(this, &OGKMenuScene::buttonPressed);

    mSettingsButton = mMainPanel->makeButton(5, vp->getActualHeight() - 35, 120, 30, "SETTINGS");
    mSettingsButton->setPressedCallback(this, &OGKMenuScene::buttonPressed);
}

void OGKMenuScene::_createLogo()
{
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    
    Gorilla::Layer *layer =  mScreen->createLayer();
    Ogre::Real x = MAX(0,vp->getActualWidth() - 230 - 20);
    Ogre::Real y = MAX(0,vp->getActualHeight() - 92 - 20);
    Gorilla::Rectangle *logo = layer->createRectangle(x,y);
    logo->background_image("logo_badge");
    logo->width(230);
    logo->height(92);
}

void OGKMenuScene::_createSettingsMenu()
{
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    mSettingsPanel = OGRE_NEW Gui3D::ScreenRenderable2DPanel(mGUI,
                                                         mScreen,
                                                         Ogre::Vector2(0,0),
                                                         Ogre::Vector2(vp->getActualWidth(),
                                                                       vp->getActualHeight()),
                                                         "default_theme",
                                                         "SettingsMenu");
    mSettingsPanel->showInternalMousePointer();
    
    mBackButton = mSettingsPanel->makeButton(5, 5, 120, 30, "BACK");
    mBackButton->setPressedCallback(this, &OGKMenuScene::buttonPressed);
    
    Ogre::Real centerX = vp->getActualWidth() / 2 ;
    Ogre::Real centerY = vp->getActualHeight() / 2 ;
    
    mFullScreenCaption = mSettingsPanel->makeCaption(centerX - 150, centerY, 150, 30, "Full screen");
    
    mFullScreenCheckbox = (Gui3D::CheckboxText *)mSettingsPanel->makeCheckbox(centerX + 10, centerY, 30, 30);
    mFullScreenCheckbox->setSelecteStateChangedCallback(this, &OGKMenuScene::stateChanged);
    mFullScreenCheckbox->setChecked(OGKGame::getSingletonPtr()->mRenderWindow->isFullScreen());
    
    Ogre::StringVector renderDevices;
    Ogre::StringVector resolutions;
    
    // store available rendering devices and available resolutions
    Ogre::ConfigOptionMap& CurrentRendererOptions = Ogre::Root::getSingletonPtr()->getRenderSystem()->getConfigOptions();
    Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin();
    while( configItr != CurrentRendererOptions.end() ) {
        if( (configItr)->first == "Rendering Device" )
        {
            // Store Available Rendering Devices
            renderDevices = ((configItr)->second.possibleValues);
        }
        if( (configItr)->first == "Video Mode" )
        {
            // Store Available Resolutions
            resolutions = ((configItr)->second.possibleValues);
        }
        configItr++;
    }
    
    // make a combo box of resolutions
    
    mSettingsPanel->setVisible(false);
}



