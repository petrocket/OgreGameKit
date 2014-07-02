//
//  OGKInGameScene.cpp
//  OgreGameKit
//
//  Created by Alex on 6/24/14.
//
//

#include "OGKInGameScene.h"

#include "OGKGame.h"

// DEFAULT THEME
class Gui3DHUDTheme : public Gui3D::PanelColors
{
public:
    inline Gui3DHUDTheme():Gui3D::PanelColors()
    {
        transparent = Gorilla::rgb(0,0,0,0);
        
        // Panel
        panelBackgroundSpriteName = "panelBackground";
        panelCursorSpriteName = "mousepointer";
        panelCursorSpriteSizeX = 12;
        panelCursorSpriteSizeY = 18;
        
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
    }
};

static Gui3DHUDTheme gui3DHUDTheme;

OGKInGameScene::OGKInGameScene(const Ogre::String& name):OGKScene(name),
    mGUI(NULL),
    mMenuPanel(NULL),
    mPlayer(NULL),
    mTerrain(NULL)
{
    init();
}

OGKInGameScene::~OGKInGameScene()
{
    
}

bool OGKInGameScene::buttonPressed(Gui3D::PanelElement *e)
{
    if(e == mPauseButton) {
        mMenuPanel->setVisible(true);
        mHUDPanel->setVisible(false);
        return false;
    }
    else if(e == mMenuButton) {
        OGKGame::getSingletonPtr()->mGameSceneManager->setActiveScene("menu", 500);
        return false;
    }
    else if(e == mCloseDialogButton) {
        mDialogPanel->setVisible(false);
        mHUDPanel->setVisible(true);
        return false;
    }
    else if(e == mResumeButton) {
        mMenuPanel->setVisible(false);
        mHUDPanel->setVisible(true);
    }
    return true;
}

void OGKInGameScene::init()
{
    // don't call OGKScene init (it's constructor already does that)
//    OGKScene::init();
    
    mGUI = OGRE_NEW Gui3D::Gui3D(&gui3DHUDTheme);
}

void OGKInGameScene::onEnter()
{
    OGKScene::onEnter();
    
	mSceneManager->setSkyBox(true, "OGK/DefaultSkyBox");
    
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    mScreen = mGUI->createScreenRenderable2D(vp, "default_theme","ingame");
    Ogre::SceneNode *node = OGRE_NEW Ogre::SceneNode(mSceneManager);
    node->attachObject(mScreen);
    mOverlay->add3D(node);
    mOverlay->show();
    
    _initLoadingPanel();
    _initMenuPanel();
    _initHUDPanel();
    _initDialogPanel();
    
    mLoadingPanel->setVisible(true);
    
    // terrain
    Ogre::Vector3 lightDir(0.55,-0.3,0.75);
    lightDir.normalise();
    mLight = mSceneManager->createLight("Light");
    mLight->setType(Ogre::Light::LT_DIRECTIONAL);
    mLight->setDirection(lightDir);
    mLight->setDiffuseColour(Ogre::ColourValue::White);
    
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.1,0.15,0.4));
    Ogre::ColourValue fogColour(184.0/255.0, 223.0/255.0, 251.0/255.0);
    mSceneManager->setFog(Ogre::FOG_LINEAR, fogColour, 0.0, 1000, 4000);

    mCamera->setTarget(NULL);
    mCamera->setMode(OGKCamera::FIXED);
    mCamera->setPosition(Ogre::Vector3(0,1000,0));

    //playBackgroundMusic("media/audio/background.mp3");
    
    OGKInputManager::getSingletonPtr()->addKeyListener(this, "ingameScene");
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "ingameScene");

    Ogre::CompositorInstance *inst = Ogre::CompositorManager::getSingleton().addCompositor(vp, "Bloom");
    if(!inst) {
        OGKLOG("Failed to instanciate bloom compositor");
    }
    else {
        Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(vp,"Bloom",true);
    }
}

void OGKInGameScene::onEnterTransitionDidFinish()
{
    OGKScene::onEnterTransitionDidFinish();
    
    // @TODO Load from .scene file instead?
    mTerrain = OGRE_NEW OGKTerrain();
    mTerrain->setup(mSceneManager, mLight);
    
    mPlayer = OGRE_NEW OGKPlayer(mSceneManager);

    mCamera->setTarget(mPlayer->getSceneNode());
    mCamera->setMode(OGKCamera::THIRD_PERSON_INDIRECT);
//    mCamera->setMode(OGKCamera::FREE);
}

void OGKInGameScene::onExit()
{
    // delete the player first because it might remove some nodes
    if(mPlayer) {
        OGRE_DELETE mPlayer;
        mPlayer = NULL;
    }

    if(mTerrain) {
        OGRE_DELETE mTerrain;
        mTerrain = NULL;
    }
    
    OGKScene::onExit();
    
    if(mGUI) mGUI->destroyScreenRenderable2D("ingame");
    
    mScreen = NULL;
    
    if(mCamera) mCamera->setTarget(NULL);
}

void OGKInGameScene::onExitTransitionDidStart()
{
    OGKScene::onExitTransitionDidStart();
    
    stopBackgroundMusic();
    
    if(mMenuPanel) mMenuPanel->hideInternalMousePointer();
    
    OGKInputManager::getSingletonPtr()->removeKeyListener("ingameScene");
    OGKInputManager::getSingletonPtr()->removeMouseListener("ingameScene");
}

void OGKInGameScene::update(Ogre::Real elapsedTime)
{
    OGKScene::update(elapsedTime);
    
    if(mTerrain) {
        mTerrain->update();
        
        if(mLoadingPanel->isVisible()) {
            Ogre::Terrain *terrain = mTerrain->mTerrainGroup->getTerrain(0, 0);
            if(terrain->isLoaded()) {
                mLoadingPanel->setVisible(false);
                mHUDPanel->setVisible(true);
                if(mPlayer) mPlayer->setEnabled(true);
            }
        }
    }
    
    if(mPlayer) {
        mPlayer->update(elapsedTime);
    }
}

bool OGKInGameScene::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    switch (keyEventRef.key) {
        case OIS::KC_ESCAPE:
            if(mMenuPanel->isVisible()) {
                mMenuPanel->setVisible(false);
                mCamera->setMode(OGKCamera::THIRD_PERSON_INDIRECT);
            }
            else {
                mMenuPanel->setVisible(true);
                mCamera->setMode(OGKCamera::FIXED);
            }
            break;
        default:
            break;
    }
    
    return true;
}


#ifdef OGRE_IS_IOS
bool OGKInGameScene::touchMoved(const OIS::MultiTouchEvent &evt) { return true; }
bool OGKInGameScene::touchPressed(const OIS::MultiTouchEvent &evt)  { return true; }
bool OGKInGameScene::touchReleased(const OIS::MultiTouchEvent &evt)  { return true; }
bool OGKInGameScene::touchCancelled(const OIS::MultiTouchEvent &evt)  { return true; }
#else
bool OGKInGameScene::mouseMoved(const OIS::MouseEvent &evt)
{
    if(mMenuPanel) mMenuPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);
    if(mHUDPanel) mHUDPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);
    if(mDialogPanel) mDialogPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);
    
    if(mMenuPanel->isVisible()) {
        // @TODO
    }
    else if(mDialogPanel->isVisible()) {
        // @TODO
    }
    else if(mCamera && mCamera->getMode() == OGKCamera::THIRD_PERSON_INDIRECT && mTerrain &&
            !mHUDPanel->getFocusedElement()) {
        if(evt.state.buttonDown(OIS::MB_Left)) {
            Ogre::Real x = (float)evt.state.X.abs / (float)evt.state.width;
            Ogre::Real y = (float)evt.state.Y.abs / (float)evt.state.height;
            
            //const OIS::MouseState &mouseState = OGKInputManager::getSingletonPtr()->getMouse()->getMouseState();
            
            //            mLog->logMessage("click abs " +
            //                               Ogre::StringConverter::toString(evt.state.X.abs) + " " +
            //                               Ogre::StringConverter::toString(evt.state.Y.abs) + " rel " +
            //                               Ogre::StringConverter::toString(x) + " " +
            //                               Ogre::StringConverter::toString(y));
            // set the player destination
            Ogre::Ray ray = mCamera->getCamera()->getCameraToViewportRay(x,y);
            Ogre::TerrainGroup::RayResult rayResult=mTerrain->mTerrainGroup->rayIntersects(ray);
            if(rayResult.hit) {
                mPlayer->setDestination(rayResult.position);
            }
        }
    }
    return false;
}

bool OGKInGameScene::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mMenuPanel && mMenuPanel->isVisible()) mMenuPanel->injectMousePressed(evt, id);
    if(mHUDPanel && mHUDPanel->isVisible()) {
        mHUDPanel->injectMousePressed(evt, id);
    }
    if(mDialogPanel && mDialogPanel->isVisible()) mDialogPanel->injectMousePressed(evt, id);
    
    if(mMenuPanel->isVisible()) {
        // @TODO
    }
    else if(mDialogPanel->isVisible()) {
        // @TODO
    }
    else if(mCamera && mCamera->getMode() == OGKCamera::THIRD_PERSON_INDIRECT && mTerrain &&
       !mHUDPanel->getFocusedElement()) {
        
        Ogre::Real x = (float)evt.state.X.abs / (float)evt.state.width;
        Ogre::Real y = (float)evt.state.Y.abs / (float)evt.state.height;
        
        //const OIS::MouseState &mouseState = OGKInputManager::getSingletonPtr()->getMouse()->getMouseState();
        
        //        mLog->logMessage("click abs " +
        //                           Ogre::StringConverter::toString(evt.state.X.abs) + " " +
        //                           Ogre::StringConverter::toString(evt.state.Y.abs) + " rel " +
        //                           Ogre::StringConverter::toString(x) + " " +
        //                           Ogre::StringConverter::toString(y));
        // set the player destination
        Ogre::Ray ray = mCamera->getCamera()->getCameraToViewportRay(x,y);
        Ogre::TerrainGroup::RayResult rayResult=mTerrain->mTerrainGroup->rayIntersects(ray);
        if(rayResult.hit) {
            mPlayer->setDestination(rayResult.position);
        }
    }
    return false;
}

bool OGKInGameScene::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mMenuPanel && mMenuPanel->isVisible()) mMenuPanel->injectMouseReleased(evt, id);
    if(mHUDPanel && mHUDPanel->isVisible()) mHUDPanel->injectMouseReleased(evt, id);
    if(mDialogPanel && mDialogPanel->isVisible()) mDialogPanel->injectMouseReleased(evt, id);
    
    return false;
}
#endif

#pragma mark - Private

void OGKInGameScene::_initLoadingPanel()
{
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    Ogre::Real centerX = vp->getActualWidth() / 2 ;
    Ogre::Real centerY = vp->getActualHeight() / 2 ;
    
    mLoadingPanel  = OGRE_NEW Gui3D::ScreenRenderable2DPanel(mGUI,
                                                            mScreen,
                                                            Ogre::Vector2(0,0),
                                                            Ogre::Vector2(vp->getActualWidth(),
                                                                          vp->getActualHeight()),
                                                            "default_theme",
                                                            "LoadingPanel");
    mLoadingPanel->getBackground()->background_image(NULL);
    mLoadingPanel->getBackground()->background_colour(Gorilla::Colours::Black);
    mLoadingPanel->hideInternalMousePointer();
    mLoadingPanel->setVisible(false);
    
    mLoadingCaption = mLoadingPanel->makeCaption(centerX - 150, centerY - 10,
                                                   300, 20, "LOADING...");
    mLoadingCaption->getCaption()->align(Gorilla::TextAlign_Centre);
    mLoadingCaption->getCaption()->font(14);
}

void OGKInGameScene::_initMenuPanel()
{
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    Ogre::Real centerX = vp->getActualWidth() / 2 ;
    Ogre::Real centerY = vp->getActualHeight() / 2 ;

    mMenuPanel = OGRE_NEW Gui3D::ScreenRenderable2DPanel(mGUI,
                                                        mScreen,
                                                         Ogre::Vector2(0,0),
                                                         Ogre::Vector2(vp->getActualWidth(),
                                                                       vp->getActualHeight()),
                                                        "default_theme",
                                                        "MenuPanel");
    mMenuPanel->getBackground()->background_image(NULL);
    mMenuPanel->getBackground()->background_colour(Gorilla::rgb(0,0,0,0));
    mMenuPanel->showInternalMousePointer();
    mMenuPanel->setVisible(false);
    
    mMenuButton = mMenuPanel->makeButton(5, 5, 200, 30, "MENU");
    mMenuButton->setPressedCallback(this, &OGKInGameScene::buttonPressed);
    mResumeButton = mMenuPanel->makeButton(5, 45, 200, 30, "RESUME");
    mResumeButton->setPressedCallback(this, &OGKInGameScene::buttonPressed);
}

void OGKInGameScene::_initHUDPanel()
{
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    Ogre::Real centerX = vp->getActualWidth() / 2 ;
    Ogre::Real centerY = vp->getActualHeight() / 2 ;
    
    mHUDPanel = OGRE_NEW Gui3D::ScreenRenderable2DPanel(mGUI,
                                                         mScreen,
                                                         Ogre::Vector2(0,0),
                                                         Ogre::Vector2(vp->getActualWidth(),
                                                                       vp->getActualHeight()),
                                                         "default_theme",
                                                         "HUDPanel");
    mHUDPanel->getBackground()->background_image(NULL);
    mHUDPanel->getBackground()->background_colour(Gorilla::rgb(0,0,0,0));
    mHUDPanel->showInternalMousePointer();
    mHUDPanel->setVisible(false);
    
    mPauseButton = mHUDPanel->makeButton(5, 5, 200, 30, "PAUSE");
    mPauseButton->setPressedCallback(this, &OGKInGameScene::buttonPressed);
}

void OGKInGameScene::_initDialogPanel()
{
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    Ogre::Real centerX = vp->getActualWidth() / 2 ;
    Ogre::Real centerY = vp->getActualHeight() / 2 ;
    
    mDialogPanel = OGRE_NEW Gui3D::ScreenRenderable2DPanel(mGUI,
                                                        mScreen,
                                                        Ogre::Vector2(50,vp->getActualHeight() - 150),
                                                        Ogre::Vector2(vp->getActualWidth() - 100,
                                                                      vp->getActualHeight() - 150),
                                                        "default_theme",
                                                        "DialogPanel");
    mDialogPanel->showInternalMousePointer();
    mDialogPanel->setVisible(false);
    
    mCloseDialogButton = mDialogPanel->makeButton(vp->getActualWidth() - 200,
                                                  vp->getActualHeight() - 80,
                                                  180, 40, "OK");
    mCloseDialogButton->setPressedCallback(this, &OGKInGameScene::buttonPressed);
}