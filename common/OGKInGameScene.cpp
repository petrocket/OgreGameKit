//
//  OGKInGameScene.cpp
//  OgreGameKit
//
//  Created by Alex on 6/24/14.
//
//

#include "OGKInGameScene.h"
#include "OGKGame.h"
#include "OGKGUIThemes.h"

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
        mDialogPanel->setVisible(false);
        return false;
    }
    else if(e == mMenuButton) {
        OGKGame::getSingletonPtr()->mGameSceneManager->setActiveScene("menu", 500);
        return false;
    }
    else if(e == mCloseDialogButton) {
        mDialogPanel->setVisible(false);
        mHUDPanel->setVisible(true);
        
        if(!mPlayer->isAlive()) {
            OGKGame::getSingletonPtr()->mGameSceneManager->setActiveScene("menu", 500);
        }
        return false;
    }
    else if(e == mResumeButton) {
        mMenuPanel->setVisible(false);
        mHUDPanel->setVisible(true);
    }
#ifdef OGRE_IS_IOS
    else if(e == mCamModeButton) {
        if(mCamModeButton->getCaption()->text().compare("FREE CAM") == 0) {
            mCamera->setMode(OGKCamera::FREE);
            if(mPlayer) mPlayer->setEnabled(false);
            mCamModeButton->getCaption()->text("3RD PERSON");
        }
        else {
            mCamera->setMode(OGKCamera::THIRD_PERSON_INDIRECT);
            if(mPlayer) mPlayer->setEnabled(true);
            mCamModeButton->getCaption()->text("FREE CAM");
        }
    }
#endif
    return true;
}

void OGKInGameScene::init()
{
    // don't call OGKScene init (it's constructor already does that)
//    OGKScene::init();
    
    mGUI = OGRE_NEW Gui3D::Gui3D(OGKGame::getSingleton().mDefaultGUITheme);
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
    
    // light
    Ogre::Vector3 lightDir(1,-1,1);
    lightDir.normalise();
    mLight = mSceneManager->createLight("Light");
    mLight->setType(Ogre::Light::LT_DIRECTIONAL);
    mLight->setDirection(lightDir);
    mLight->setDiffuseColour(Ogre::ColourValue::White);
    
//    mSceneManager->setAmbientLight(Ogre::ColourValue(0.1,0.15,0.4));
//    Ogre::ColourValue fogColour(184.0/255.0, 223.0/255.0, 251.0/255.0);
//    mSceneManager->setFog(Ogre::FOG_LINEAR, fogColour, 0.0, 1000, 4000);

    mCamera->setTarget(NULL);
    mCamera->setMode(OGKCamera::FIXED);
    mCamera->setPosition(Ogre::Vector3(0,1000,0));
    
    OGKInputManager::getSingletonPtr()->addKeyListener(this, "ingameScene");
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->addMultiTouchListener(this, "ingameScene");
#else
    OGKInputManager::getSingletonPtr()->addMouseListener(this, "ingameScene");
#endif

#ifndef OGRE_IS_IOS
    Ogre::CompositorInstance *inst = Ogre::CompositorManager::getSingleton().addCompositor(vp, "Bloom");
    if(!inst) {
        OGKLOG("Failed to instanciate bloom compositor");
    }
    else {
        Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(vp,"Bloom",true);
    }
#endif
}

void OGKInGameScene::onEnterTransitionDidFinish()
{
    OGKScene::onEnterTransitionDidFinish();
    
    loadFromFile("level1.scene");
    
    // @TODO Load from .scene file instead?
    mTerrain = OGRE_NEW OGKTerrain();
    mTerrain->setup(mSceneManager, mLight);
    
    mPlayer = OGRE_NEW OGKPlayer(mSceneManager);
    mPlayer->setHealth(100,100);

    Ogre::SceneManager::MovableObjectIterator ii = mSceneManager->getMovableObjectIterator("Entity");
    while(ii.hasMoreElements()) {
        Ogre::Entity *entity =  static_cast<Ogre::Entity*>(ii.getNext());
        if(Ogre::StringUtil::startsWith(entity->getName(), "npc")) {
            OGKNPC *npc =OGRE_NEW OGKNPC(entity, entity->getParentSceneNode());
            mNPCs.push_back(npc);
        }
        else if(Ogre::StringUtil::startsWith(entity->getName(), "enemy")) {
            OGKNPC *npc =OGRE_NEW OGKNPC(entity, entity->getParentSceneNode());
            npc->setIsEnemy(true);
            mNPCs.push_back(npc);
        }
    }
    
    mCamera->setTarget(mPlayer->getSceneNode());
    mCamera->setMode(OGKCamera::THIRD_PERSON_INDIRECT);
    mCamera->setTargetOffset(Ogre::Vector3(15,30,15));
}

void OGKInGameScene::onExit()
{
    // delete the player first because it might remove some nodes
    if(mPlayer) {
        OGRE_DELETE mPlayer;
        mPlayer = NULL;
    }

    if(mNPCs.size()) {
        for(int i = 0; i < mNPCs.size(); i++) {
            OGRE_DELETE mNPCs[i];
        }
        mNPCs.clear();
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
    
    OGKInputManager::getSingletonPtr()->removeKeyListener(this);
#ifdef OGRE_IS_IOS
    OGKInputManager::getSingletonPtr()->removeMultiTouchListener(this);
#else
    OGKInputManager::getSingletonPtr()->removeMouseListener(this);
#endif
}

void OGKInGameScene::update(Ogre::Real elapsedTime)
{
    bool isLoading = mLoadingPanel->isVisible();
    
    if(mTerrain) {
        mTerrain->update(elapsedTime);
        
        if(isLoading) {
            Ogre::Terrain *terrain = mTerrain->mTerrainGroup->getTerrain(0, 0);
            if(terrain->isLoaded() &&
               !mTerrain->mTerrainGroup->isDerivedDataUpdateInProgress()) {
                playBackgroundMusic("media/audio/background.mp3");
                
                mLoadingPanel->setVisible(false);
                mHUDPanel->setVisible(true);
                if(mPlayer) mPlayer->setEnabled(true);
            }
        }
    }
    
    if(!isLoading) {
        if(mNPCs.size()) {
            for(int i = 0; i < mNPCs.size(); i++) {
                mNPCs[i]->update(elapsedTime);
            }
        }
        
        // update the player before the camera yo or camera snap ain't perfect
        if(mPlayer) {
            mPlayer->update(elapsedTime);
            _updateHUD();
        }
    }
    
    OGKScene::update(elapsedTime);
}

bool OGKInGameScene::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    switch (keyEventRef.key) {
        case OIS::KC_ESCAPE:
            if(mMenuPanel->isVisible()) {
                OGKGame::getSingletonPtr()->mGameSceneManager->setActiveScene("menu", 500);
            }
            else {
                mMenuPanel->setVisible(true);
                mHUDPanel->setVisible(false);
            }
            return false;
        case OIS::KC_C:
            if(mMenuPanel && !mMenuPanel->isVisible()) {
                if(mCamera->getMode() == OGKCamera::FREE) {
                    mCamera->setMode(OGKCamera::THIRD_PERSON_INDIRECT);
                    mHUDPanel->showInternalMousePointer();
                    if(mPlayer) mPlayer->setEnabled(true);
                    if(mTerrain) mTerrain->setMode(OGKTerrain::MODE_NORMAL);
                }
                else {
                    mCamera->setMode(OGKCamera::FREE);
                    mHUDPanel->hideInternalMousePointer();
                    if(mPlayer) mPlayer->setEnabled(false);
                    if(mTerrain) mTerrain->setMode(OGKTerrain::MODE_EDIT_HEIGHT);
                }
            }
            return false;
        case OIS::KC_M:
        {
            static int mode = 0;
            
            if(mode == 2)
            {
                mCamera->getCamera()->setPolygonMode(Ogre::PM_SOLID);
                mode = 0;
            }
            else if(mode == 0)
            {
                mCamera->getCamera()->setPolygonMode(Ogre::PM_WIREFRAME);
                mode = 1;
            }
            else if(mode == 1)
            {
                mCamera->getCamera()->setPolygonMode(Ogre::PM_POINTS);
                mode = 2;
            }
            return false;
        }
        case OIS::KC_P:
            OGKGame::getSingletonPtr()->mRenderWindow->writeContentsToTimestampedFile("OGK_", ".png");
            break;
        default:
            break;
    }
    
    return true;
}


#ifdef OGRE_IS_IOS
bool OGKInGameScene::touchMoved(const OIS::MultiTouchEvent &evt) { return true; }
bool OGKInGameScene::touchPressed(const OIS::MultiTouchEvent &evt)  {
    
    if(mMenuPanel && mMenuPanel->isVisible()) {
        mMenuPanel->injectTouchPressed(evt);
    }
    else {
        if(mDialogPanel && mDialogPanel->isVisible()) {
            mDialogPanel->injectTouchPressed(evt);
        }
        if(mHUDPanel && mHUDPanel->isVisible() && !mDialogPanel->getFocusedElement()) {
            mHUDPanel->injectTouchPressed(evt);
        }
    }

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
        
        _handleClickEvent(x,y);
    }
    
    return false;
}
    
bool OGKInGameScene::touchReleased(const OIS::MultiTouchEvent &evt)  { return true; }
bool OGKInGameScene::touchCancelled(const OIS::MultiTouchEvent &evt)  { return true; }
#else
bool OGKInGameScene::mouseMoved(const OIS::MouseEvent &evt)
{
    if(mDialogPanel) mDialogPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);
    if(mMenuPanel) mMenuPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);
    if(mHUDPanel) mHUDPanel->injectMouseMoved(evt.state.X.abs, evt.state.Y.abs);

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

            // set the player destination
            Ogre::Ray ray = mCamera->getCamera()->getCameraToViewportRay(x,y);
            Ogre::TerrainGroup::RayResult rayResult=mTerrain->mTerrainGroup->rayIntersects(ray);
            if(rayResult.hit) {
                // attack if SHIFT is down
                if(OGKInputManager::getSingletonPtr()->getKeyboard()->isKeyDown(OIS::KC_LSHIFT)) {
                    mPlayer->attack(rayResult.position);
                }
                else {
                    mPlayer->setDestination(rayResult.position);
                }
            }
        }
    }
    return false;
}

bool OGKInGameScene::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mMenuPanel && mMenuPanel->isVisible()) {
        mMenuPanel->injectMousePressed(evt, id);
    }
    else {
        if(mDialogPanel && mDialogPanel->isVisible()) {
            mDialogPanel->injectMousePressed(evt, id);
        }
        if(mHUDPanel && mHUDPanel->isVisible() && !mDialogPanel->getFocusedElement()) {
            mHUDPanel->injectMousePressed(evt, id);
        }
    }
    
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
            
            _handleClickEvent(x,y);
        }
    }
    
    return false;
}

bool OGKInGameScene::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mMenuPanel && mMenuPanel->isVisible()) {
        mMenuPanel->injectMouseReleased(evt, id);
    }
    else {
        if(mDialogPanel && mDialogPanel->isVisible()) {
            mDialogPanel->injectMouseReleased(evt, id);
        }
        if(mHUDPanel && mHUDPanel->isVisible() && !mDialogPanel->getFocusedElement()) {
            mHUDPanel->injectMouseReleased(evt, id);
        }
    }
    
    return false;
}
#endif

#pragma mark - Private

OGKNPC *OGKInGameScene::getNPC(Ogre::Entity *entity)
{
    // is this an npc?
    for(int i = 0; i < mNPCs.size(); i++) {
        if(entity == mNPCs[i]->mEntity) {
            return mNPCs[i];
        }
    }
    return NULL;
}

void OGKInGameScene::_handleClickEvent(Ogre::Real x, Ogre::Real y)
{
    // set the player destination
    Ogre::Ray ray = mCamera->getCamera()->getCameraToViewportRay(x,y);
    
    bool checkTerrain = true;
    
    // does it intersect an entity?
    Ogre::RaySceneQuery *query = mSceneManager->createRayQuery(ray);
    Ogre::RaySceneQueryResult& sceneResult = query->execute();
    if(sceneResult.size()) {
        Ogre::RaySceneQueryResult::iterator itr = sceneResult.begin();
        for(;itr != sceneResult.end(); itr++) {
            if(itr->movable && itr->movable != mPlayer->mEntity) {
                OGKNPC *npc = getNPC((Ogre::Entity *)itr->movable);
                if(npc) {
                    if(npc->isEnemy()) {
                        mPlayer->attack(npc->mEntity);
                        npc->damage(10.0);
                    }
                    else {
                        interactWithNPC(npc);
                    }
                    checkTerrain = false;
                    break;
                }
            }
        }
    }
    mSceneManager->destroyQuery(query);
    
    if(checkTerrain) {
        Ogre::TerrainGroup::RayResult rayResult=mTerrain->mTerrainGroup->rayIntersects(ray);
        if(rayResult.hit) {
            // attack if SHIFT is down
#ifndef OGRE_IS_IOS
            if(OGKInputManager::getSingletonPtr()->getKeyboard()->isKeyDown(OIS::KC_LSHIFT)) {
                mPlayer->attack(rayResult.position);
            }
            else {
#endif
                mPlayer->setDestination(rayResult.position);
#ifndef OGRE_IS_IOS
            }
#endif
        }
    }
}

void OGKInGameScene::interactWithNPC(OGKNPC *npc)
{
    mDialogCaption->getCaption()->text("Hi! I'm an NPC!");
    mDialogPanel->setVisible(true);
}

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
    mLoadingCaption->getCaption()->colour(Ogre::ColourValue(1,1,1,0.4));
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
    mMenuPanel->getBackground()->background_colour(Gorilla::rgb(0,0,0,10));
#ifdef OGRE_IS_IOS
    mMenuPanel->hideInternalMousePointer();
#else
    mMenuPanel->showInternalMousePointer();
#endif
    mMenuPanel->setVisible(false);
    
#ifdef OGRE_IS_IOS
    Ogre::Real buttonHeight = 60.0;
    Ogre::Real buttonWidth = 400.0;
#else
    Ogre::Real buttonHeight = 40.0;
    Ogre::Real buttonWidth = 200.0;
#endif
    
    mMenuButton = mMenuPanel->makeButton(mMenuPanel->getBackground()->width() / 2 - buttonWidth / 2,
                                         mMenuPanel->getBackground()->height() / 2 - buttonHeight - 10,
                                         buttonWidth, buttonHeight, "MENU");
    mMenuButton->setPressedCallback(this, &OGKInGameScene::buttonPressed);
    mResumeButton = mMenuPanel->makeButton(mMenuPanel->getBackground()->width() / 2 - buttonWidth / 2,
                                           mMenuPanel->getBackground()->height() / 2 + 10,
                                           buttonWidth, buttonHeight, "RESUME");
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
    mHUDPanel->setVisible(false);

#ifdef OGRE_IS_IOS
    mHUDPanel->hideInternalMousePointer();
    Ogre::Real buttonHeight = 50.0;
#else
    mHUDPanel->showInternalMousePointer();
    Ogre::Real buttonHeight = 30.0;
#endif
    
    Ogre::Real top = vp->getActualHeight() - 40;
    mHeart1 = mHUDPanel->getGUILayer()->createRectangle(10, top, 32, 32);
    mHeart2 = mHUDPanel->getGUILayer()->createRectangle(50, top, 32, 32);
    mHeart3 = mHUDPanel->getGUILayer()->createRectangle(90, top, 32, 32);
    mHeart1->background_image("heart_full");
    mHeart2->background_image("heart_full");
    mHeart3->background_image("heart_full");
    
    mPauseButton = mHUDPanel->makeButton(5, 5, 200, buttonHeight, "PAUSE");
    mPauseButton->setPressedCallback(this, &OGKInGameScene::buttonPressed);
    
#ifdef OGRE_IS_IOS
    Ogre::Real right = vp->getActualWidth() - 200 - 5;
    mCamModeButton = mHUDPanel->makeButton(right, 5, 200, buttonHeight, "FREE CAM");
    mCamModeButton->setPressedCallback(this, &OGKInGameScene::buttonPressed);
#endif
}

void OGKInGameScene::_initDialogPanel()
{
    Ogre::Viewport *vp = mCamera->getCamera()->getViewport();
    Ogre::Real centerX = vp->getActualWidth() / 2 ;
    Ogre::Real centerY = vp->getActualHeight() / 2 ;
    Ogre::Real margin = 100;
    Ogre::Real width = 620;
    Ogre::Real height = 140;
    
    mDialogPanel = OGRE_NEW Gui3D::ScreenRenderable2DPanel(mGUI,
                                                        mScreen,
                                                        Ogre::Vector2(vp->getActualWidth() / 2 - width / 2,
                                                                      vp->getActualHeight() - height - 20),
                                                        Ogre::Vector2(width,height),
                                                        "default_theme",
                                                        "DialogPanel");
    
#ifdef OGRE_IS_IOS
    mDialogPanel->hideInternalMousePointer();
#else
    mDialogPanel->showInternalMousePointer();
#endif
    mDialogPanel->setVisible(false);
    mDialogPanel->getBackground()->background_image(NULL);
    mDialogPanel->getBackground()->background_colour(Gorilla::rgb(0,0,0,80));
    
    mDialogCaption = mDialogPanel->makeCaption(10, 10,
                                               width - 240,
                                               height - 20,
                                               "");
    mCloseDialogButton = mDialogPanel->makeButton(width - 200,
                                                  height - 80,
                                                  180, 40, "OK");
    mCloseDialogButton->setPressedCallback(this, &OGKInGameScene::buttonPressed);
}

void OGKInGameScene::_updateHUD()
{
    if(mPlayer && mHUDPanel &&
       mHUDPanel->isVisible() &&
       mLastHealth != mPlayer->getHealth() &&
       mPlayer->getMaxHealth() > 0.001) {
        
        bool justDied = mLastHealth > 0.001 && !mPlayer->isAlive();
        
        Ogre::Real health = MAX(0.0,MIN(1.0,mPlayer->getHealth() / mPlayer->getMaxHealth()));
        health *= 6.0;
        health = MAX(0.0, MIN(6.0,health)); // paranoia
        
        int fullAmount = (int)ceil(health);
        
        mHeart3->background_image(fullAmount > 5 ? "heart_full" : (fullAmount > 4 ? "heart_half_full" : "heart_empty"));
        mHeart2->background_image(fullAmount > 3 ? "heart_full" : (fullAmount > 2 ? "heart_half_full" : "heart_empty"));
        mHeart1->background_image(fullAmount > 1 ? "heart_full" : (fullAmount > 0 ? "heart_half_full" : "heart_empty"));
        
        mLastHealth = mPlayer->getHealth();
        
        if(justDied) {
            mDialogPanel->setVisible(true);
            mDialogCaption->text("GAME OVER");
        }
    }
}