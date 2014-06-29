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

#include "Gui3D.h"
#include "Gui3DButton.h"
#include "Gui3DScreenRenderable2DPanel.h"

class OGKInGameScene : public OGKScene
{
public:
    OGKInGameScene(const Ogre::String& name);
    ~OGKInGameScene();
    
    virtual void init();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    virtual void onExitTransitionDidStart();
    
    virtual void update(Ogre::Real elapsedTime);
    
    // input
#ifdef OGRE_IS_IOS
	virtual bool touchMoved(const OIS::MultiTouchEvent &evt);
	virtual bool touchPressed(const OIS::MultiTouchEvent &evt);
	virtual bool touchReleased(const OIS::MultiTouchEvent &evt);
	virtual bool touchCancelled(const OIS::MultiTouchEvent &evt);
#else
	virtual bool mouseMoved(const OIS::MouseEvent &evt);
	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
#endif
    
private:
    Gui3D::Gui3D *mGUI;
    Gui3D::ScreenRenderable2DPanel *mPanel;
    Gui3D::Button *mButton;
    Gorilla::Layer *mMouseLayer;
    Gorilla::Rectangle *mMouseCursor;
    
    bool buttonPressed(Gui3D::PanelElement *e);
};

#endif /* defined(__OgreGameKit__OGKInGameScene__) */
