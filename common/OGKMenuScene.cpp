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

static Gui3DTheme gui3DTheme;

OGKMenuScene::OGKMenuScene(const Ogre::String& name):OGKScene(name),
    mGUI(NULL),
    mMouseLayer(NULL),
    mPanel(NULL)
{
    init();
}

OGKMenuScene::~OGKMenuScene()
{
    
}

bool OGKMenuScene::buttonPressed(Gui3D::PanelElement *e)
{
    OGKGame::getSingletonPtr()->mGameSceneManager->setActiveScene("ingame", 500);
    
    return true;
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
    
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    
    
    // create the gui
    mScreen = mGUI->createScreenRenderable2D(vp, "default_theme","menu");
    mSceneNode->attachObject(mScreen);

    mPanel = OGRE_NEW Gui3D::ScreenRenderable2DPanel(mGUI,
                                         mScreen,
                                         Ogre::Vector2(vp->getActualWidth()/2 - 150,
                                                       vp->getActualHeight()/2 - 100),
                                         Ogre::Vector2(300,200),
                                         "default_theme",
                                         "testPanel");
    mPanel->hideInternalMousePointer();
    mButton = mPanel->makeButton(50, 10, 200, 30, "IN GAME");
    mButton->setPressedCallback(this, &OGKMenuScene::buttonPressed);
    
    // create an object
    Ogre::Entity *sphere = mSceneManager->createEntity( Ogre::SceneManager::PT_SPHERE);
    sphere->setMaterialName("BaseWhiteNoLighting");
    mSceneNode->attachObject(sphere);
    mSceneNode->setScale(0.05, 0.05, 0.05);
    mSceneNode->setPosition(Ogre::Vector3(-3.0,0,10));

    OGKInputManager::getSingletonPtr()->addKeyListener(this, "menuScene");
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "menuScene");
    
}

void OGKMenuScene::onEnterTransitionDidFinish()
{
    OGKScene::onEnterTransitionDidFinish();
    mPanel->showInternalMousePointer();
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
    
    if(mPanel) mPanel->hideInternalMousePointer();
    
    OGKInputManager::getSingletonPtr()->removeKeyListener("menuScene");
    OGKInputManager::getSingletonPtr()->removeMouseListener("menuScene");
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
    if(mPanel) mPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);
    return false;
}

bool OGKMenuScene::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mPanel) mPanel->injectMousePressed(evt, id);
    return false;
}
bool OGKMenuScene::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mPanel) mPanel->injectMouseReleased(evt, id);
    return false;
}
#endif
