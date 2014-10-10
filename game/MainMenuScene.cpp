//
//  MainMenuScene.cpp
//  OgreGameKit
//
//  Created by Alex on 10/9/14.
//
//

#include "MainMenuScene.h"
#include "OGKGame.h"
#include "OGKGUIThemes.h"

MainMenuScene::MainMenuScene(const Ogre::String& name):OGKScene(name),
mGUI(NULL),
mMainPanel(NULL),
mQuitButton(NULL),
mSettingsPanel(NULL)
{
    init();
}

MainMenuScene::~MainMenuScene()
{
    
}

bool MainMenuScene::buttonPressed(Gui3D::PanelElement *e)
{
    if(e == mQuitButton) {
        OGKGame::getSingletonPtr()->shutdown();
    }
    else if(e == mBackButton) {
        mMainPanel->setVisible(true);
        mSettingsPanel->setVisible(false);
    }
    else if(e == mPlayButton) {
        OGKGame::getSingletonPtr()->mGameSceneManager->setActiveScene("ingame", 500);
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

void MainMenuScene::init()
{
    // don't call OGKScene init (it's constructor already does that)
    mGUI = OGRE_NEW Gui3D::Gui3D(OGKGame::getSingleton().mDefaultGUITheme);
}

void MainMenuScene::onEnter()
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
    
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->addMultiTouchListener(this, "menuScene");
#else
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "menuScene");
#endif
}

void MainMenuScene::onEnterTransitionDidFinish()
{
    OGKScene::onEnterTransitionDidFinish();
    
    if(mMainPanel) {
#ifdef OGRE_IS_IOS
        mMainPanel->hideInternalMousePointer();
#else
        mMainPanel->showInternalMousePointer();
#endif
    }
}

void MainMenuScene::onExit()
{
    OGKScene::onExit();
    
    if(mGUI) mGUI->destroyScreenRenderable2D("menu");
    mScreen = NULL;
}

void MainMenuScene::onExitTransitionDidStart()
{
    OGKScene::onExitTransitionDidStart();
    
    if(mMainPanel) {
        mMainPanel->hideInternalMousePointer();
    }
    
    OGKInputManager::getSingletonPtr()->removeKeyListener(this);
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->removeMultiTouchListener(this);
#else
    OGKInputManager::getSingletonPtr()->removeMouseListener(this);
#endif
}

bool MainMenuScene::stateChanged(Gui3D::PanelElement *e)
{
    if(e == mFullScreenCheckbox) {
        Ogre::Real width = OGKGame::getSingletonPtr()->mRenderWindow->getWidth();
        Ogre::Real height = OGKGame::getSingletonPtr()->mRenderWindow->getHeight();
        
        bool reset = false;
        
        if(mFullScreenCheckbox->getChecked() &&
           !OGKGame::getSingletonPtr()->mRenderWindow->isFullScreen()) {
            OGKGame::getSingletonPtr()->mRenderWindow->setFullscreen(true, width, height);
            reset = true;
        }
        else if(!mFullScreenCheckbox->getChecked() &&
                OGKGame::getSingletonPtr()->mRenderWindow->isFullScreen()) {
            OGKGame::getSingletonPtr()->mRenderWindow->setFullscreen(false, width, height);
            reset = true;
        }
        
        if(reset) {
            OGKInputManager::getSingletonPtr()->reset();
            
            // refresh mouse extents
            width = OGKGame::getSingletonPtr()->mRenderWindow->getWidth();
            height = OGKGame::getSingletonPtr()->mRenderWindow->getHeight();
            
            OGKInputManager::getSingletonPtr()->setWindowExtents(width,height);
            
            onExitTransitionDidStart();
            onExit();
            onEnter();
            onEnterTransitionDidFinish();
        }
    }
}

bool MainMenuScene::keyPressed(const OIS::KeyEvent &keyEventRef)
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
bool MainMenuScene::touchMoved(const OIS::MultiTouchEvent &evt) { return true; }
bool MainMenuScene::touchPressed(const OIS::MultiTouchEvent &evt)  {
    if(mMainPanel && mMainPanel->isVisible()) {
        mMainPanel->injectTouchPressed(evt);
    }
    else if(mSettingsPanel && mSettingsPanel->isVisible()) {
        mSettingsPanel->injectTouchPressed(evt);
    }
    return false;
}
bool MainMenuScene::touchReleased(const OIS::MultiTouchEvent &evt)  { return true; }
bool MainMenuScene::touchCancelled(const OIS::MultiTouchEvent &evt)  { return true; }
#else
bool MainMenuScene::mouseMoved(const OIS::MouseEvent &evt)
{
    if(mMainPanel) {
        mMainPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);
    }
    
    if(mSettingsPanel) {
        mSettingsPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);
    }
    
    return false;
}

bool MainMenuScene::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    OGKLOG("Touch at x: " + Ogre::StringConverter::toString(evt.state.X.abs) + " y: " + Ogre::StringConverter::toString(evt.state.Y.abs) + " width: " + Ogre::StringConverter::toString(evt.state.width) + " height: " + Ogre::StringConverter::toString(evt.state.height));
    
    if(mMainPanel && mMainPanel->isVisible()) {
        mMainPanel->injectMousePressed(evt, id);
    }
    else if(mSettingsPanel && mSettingsPanel->isVisible()) {
        mSettingsPanel->injectMousePressed(evt, id);
    }
    return false;
}
bool MainMenuScene::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

void MainMenuScene::_createMainMenu()
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
    
#ifndef OGRE_IS_IOS
    Ogre::Real buttonWidth = 200;
    Ogre::Real buttonHeight = 40;
#else
    Ogre::Real buttonWidth = 300;
    Ogre::Real buttonHeight = 60;
#endif
    
    mPlayButton = mMainPanel->makeButton(vp->getActualWidth() / 2 - buttonWidth / 2,
                                         vp->getActualHeight() / 2 - buttonHeight / 2,
                                         buttonWidth, buttonHeight, "PLAY");
    mPlayButton->setPressedCallback(this, &MainMenuScene::buttonPressed);
    
#ifndef OGRE_IS_IOS
    mQuitButton = mMainPanel->makeButton(5, 5, 120, 30, "QUIT");
    mQuitButton->setPressedCallback(this, &MainMenuScene::buttonPressed);
#endif
    
    mSettingsButton = mMainPanel->makeButton(5,
                                             vp->getActualHeight() - buttonHeight - 5,
                                             buttonWidth,
                                             buttonHeight,
                                             "SETTINGS");
    mSettingsButton->setPressedCallback(this, &MainMenuScene::buttonPressed);
}

void MainMenuScene::_createLogo()
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

void MainMenuScene::_createSettingsMenu()
{
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    mSettingsPanel = OGRE_NEW Gui3D::ScreenRenderable2DPanel(mGUI,
                                                             mScreen,
                                                             Ogre::Vector2(0,0),
                                                             Ogre::Vector2(vp->getActualWidth(),
                                                                           vp->getActualHeight()),
                                                             "default_theme",
                                                             "SettingsMenu");
#ifdef OGRE_IS_IOS
    mSettingsPanel->hideInternalMousePointer();
#else
    mSettingsPanel->showInternalMousePointer();
#endif
    mSettingsPanel->setVisible(false);
    
    mBackButton = mSettingsPanel->makeButton(5, 5, 120, 30, "BACK");
    mBackButton->setPressedCallback(this, &MainMenuScene::buttonPressed);
    
    Ogre::Real centerX = vp->getActualWidth() / 2 ;
    Ogre::Real centerY = vp->getActualHeight() / 2 ;
    
    mSettingsCaption = mSettingsPanel->makeCaption(centerX - 150, centerY - 80,
                                                   300, 100, "SETTINGS");
    mSettingsCaption->getCaption()->align(Gorilla::TextAlign_Centre);
    mSettingsCaption->getCaption()->font(24);
    mSettingsCaption->getCaption()->colour(Ogre::ColourValue::White);
    
#ifndef OGRE_IS_IOS
    Ogre::Real fieldY = centerY;
    mFullScreenCaption = mSettingsPanel->makeCaption(centerX - 150, fieldY, 150, 30, "Full screen");
    
    mFullScreenCheckbox = (Gui3D::CheckboxText *)mSettingsPanel->makeCheckbox(centerX + 10, fieldY, 20, 20);
    mFullScreenCheckbox->setSelecteStateChangedCallback(this, &MainMenuScene::stateChanged);
    mFullScreenCheckbox->setChecked(OGKGame::getSingletonPtr()->mRenderWindow->isFullScreen());
    
    fieldY += 40.0;
    
    Ogre::StringVector renderDevices;
    std::vector<Ogre::String> resolutions;
    
    // store available rendering devices and available resolutions
    mResolutionCaption = mSettingsPanel->makeCaption(centerX - 150, fieldY,
                                                     150, 30, "Resolution");
    
    Ogre::ConfigOptionMap& CurrentRendererOptions = Ogre::Root::getSingletonPtr()->getRenderSystem()->getConfigOptions();
    Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin();
    while( configItr != CurrentRendererOptions.end() ) {
        if( (configItr)->first == "Rendering Device" ) {
            // Store Available Rendering Devices
            renderDevices = ((configItr)->second.possibleValues);
        }
        
        if( (configItr)->first == "Video Mode" ) {
            // Store Available Resolutions
            Ogre::StringVector values = ((configItr)->second.possibleValues);
            for(Ogre::StringVector::iterator vi = values.begin(); vi != values.end(); vi++) {
                resolutions.push_back(*vi);
            }
        }
        configItr++;
    }
    
    // make a combo box of resolutions
    mResolutionCombobox = mSettingsPanel->makeCombobox(centerX + 10, fieldY,
                                                       150, 120, resolutions, 3);
    
#endif
    
    // @TODO audio controls
}
