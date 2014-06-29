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
#include "Gui3DButton.h"
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
    Gui3D::Gui3D *mGUI;
    Gui3D::ScreenRenderable2DPanel *mPanel;
    Gui3D::Button *mButton;
    Gorilla::Layer *mMouseLayer;
    Gorilla::Rectangle *mMouseCursor;
    
    bool buttonPressed(Gui3D::PanelElement *e);
};

#endif /* defined(__OgreGameKit__OGKMenuScene__) */
