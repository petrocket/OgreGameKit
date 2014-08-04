//
//  OGKCutScene.cpp
//  OgreGameKit
//
//  Created by Alex on 7/3/14.
//
//

#include "OGKCutScene.h"
#include "OGKGUIThemes.h"
#include "OGKGame.h"
#include "Ogre.h"

OGKCutScene::OGKCutScene(const Ogre::String& name, const Ogre::String &nextSceneName) :
    OGKScene(name),
    mScreenPanel(NULL),
    mCurrentCaption(0),
    mNextCaptionTime(0),
    mNextSceneName(nextSceneName)
{
    init();
}

OGKCutScene::~OGKCutScene()
{
    if(mGUI) {
        OGRE_DELETE mGUI;
        mGUI = NULL;
    }
}

void OGKCutScene::init()
{
    // don't call OGKScene init (it's constructor already does that)
    mGUI = OGRE_NEW Gui3D::Gui3D(OGKGame::getSingleton().mDefaultGUITheme);
}

void OGKCutScene::onEnter()
{
    OGKScene::onEnter();
    
    // set the camera position
    mCamera->getCamera()->setPosition(0, 0, 0);
    mCamera->getCamera()->lookAt(0,0,1);
    
    // create the gui
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    vp->setBackgroundColour(Ogre::ColourValue::Black);
    mScreen = mGUI->createScreenRenderable2D(vp, "default_theme","cutScene");
    Ogre::SceneNode *node = OGRE_NEW Ogre::SceneNode(mSceneManager);
    node->attachObject(mScreen);
    mOverlay->add3D(node);
    mOverlay->show();
    
    _createCutScene();
    
    OGKInputManager::getSingletonPtr()->addKeyListener(this, "cutScene");
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->addMultiTouchListener(this, "cutScene");
#else
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "cutScene");
#endif
    
    mCurrentCaption = 0;
    mNextCaptionTime = -1;
}

void OGKCutScene::onEnterTransitionDidFinish()
{
    OGKScene::onEnterTransitionDidFinish();
    
    mCurrentCaption = 0;
    if(mCaptions.size()) {
        mNextCaptionTime = mCaptions[mCurrentCaption].duration;
        mCaption->getCaption()->text(mCaptions[mCurrentCaption].caption);
    }
    else {
        mNextCaptionTime = 0;
    }
}

void OGKCutScene::onExit()
{
    OGKScene::onExit();
    
    if(mGUI) mGUI->destroyScreenRenderable2D("cutScene");
    
    mScreen = NULL;
    
    mCaptions.clear();
}

void OGKCutScene::onExitTransitionDidStart()
{
     OGKScene::onExitTransitionDidStart();
    
    OGKInputManager::getSingletonPtr()->removeKeyListener(this);
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->removeMultiTouchListener(this);
#else
    OGKInputManager::getSingletonPtr()->removeMouseListener(this);
#endif
}

void OGKCutScene::update(Ogre::Real elapsedTime)
{
     OGKScene::update(elapsedTime);
    
    if(mNextCaptionTime >= 0.0 && mCurrentCaption < mCaptions.size()) {
        mNextCaptionTime -= elapsedTime;
        
        if(mNextCaptionTime <= 0.0) {
            mCurrentCaption++;
            
            if(mCurrentCaption >= mCaptions.size()) {
                OGKGame::getSingletonPtr()->mGameSceneManager->setActiveScene(mNextSceneName,1000);
            }
            else {
                mNextCaptionTime = mCaptions[mCurrentCaption].duration;
                mCaption->getCaption()->text(mCaptions[mCurrentCaption].caption);
            }
        }
    }
}

// input
bool OGKCutScene::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(keyEventRef.key == OIS::KC_ESCAPE) {
        OGKGame::getSingletonPtr()->mGameSceneManager->setActiveScene(mNextSceneName,500);
        return false;
    }
    return true;
}

#ifdef OGRE_IS_IOS
bool OGKCutScene::touchMoved(const OIS::MultiTouchEvent &evt) { return true; }
bool OGKCutScene::touchPressed(const OIS::MultiTouchEvent &evt) { return true; }
bool OGKCutScene::touchReleased(const OIS::MultiTouchEvent &evt) { return true; }
bool OGKCutScene::touchCancelled(const OIS::MultiTouchEvent &evt) { return true; }
#else
bool OGKCutScene::mouseMoved(const OIS::MouseEvent &evt)
{
    return true;
}

bool OGKCutScene::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

bool OGKCutScene::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}
#endif

#pragma mark - Private

void OGKCutScene::_createCutScene()
{
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    mScreenPanel = OGRE_NEW Gui3D::ScreenRenderable2DPanel(mGUI,
                                                             mScreen,
                                                             Ogre::Vector2(0,0),
                                                             Ogre::Vector2(vp->getActualWidth(),
                                                                           vp->getActualHeight()),
                                                             "default_theme",
                                                             "SettingsMenu");
    mScreenPanel->hideInternalMousePointer();
    //mScreenPanel->setVisible(true);
    //mScreenPanel->getBackground()->background_colour(Gorilla::Colours::Black);
    
    Ogre::Real margin = 50.0;
    Ogre::Real width = vp->getActualWidth()  - margin * 2 ;
    Ogre::Real height = vp->getActualHeight() - margin * 2 ;
    
    mCaption = mScreenPanel->makeCaption(margin, margin, width, height, "");
    mCaption->getCaption()->align(Gorilla::TextAlign_Centre);
    mCaption->getCaption()->vertical_align(Gorilla::VerticalAlign_Middle);
    mCaption->getCaption()->font(24);
    
    CutSceneCaption c;
    c.duration = 2000;
    c.caption = "At first all was silent";
    mCaptions.push_back(c);
    c.caption = "";
    c.duration = 500;
    mCaptions.push_back(c);
    c.caption = "and then we heard them";
    c.duration = 2000;
    mCaptions.push_back(c);
}