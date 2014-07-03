//
//  OGKCutScene.h
//  OgreGameKit
//
//  Created by Alex on 7/3/14.
//
//

#ifndef __OgreGameKit__OGKCutScene__
#define __OgreGameKit__OGKCutScene__

#include "OGKScene.h"

#include "Gui3D.h"
#include "Gui3DCaption.h"
#include "Gui3DScreenRenderable2DPanel.h"

struct CutSceneCaption {
    Ogre::String caption;
    Ogre::Real duration;
};

class OGKCutScene : public OGKScene
{
public:
    OGKCutScene(const Ogre::String& name, const Ogre::String &nextSceneName);
    ~OGKCutScene();
    
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
    void _createCutScene();
    
    Ogre::String mNextSceneName;
    
    Gui3D::Gui3D *mGUI;
    Gui3D::ScreenRenderable2DPanel *mScreenPanel;
    Gui3D::Caption *mCaption;
    
    unsigned int mCurrentCaption;
    Ogre::Real mNextCaptionTime;
    std::vector<CutSceneCaption> mCaptions;
};

#endif /* defined(__OgreGameKit__OGKCutScene__) */
