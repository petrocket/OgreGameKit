//
//  OGKGame.h
//  OgreGameKit
//
//  Created by Alex on 5/20/14.
//
//

#ifndef __OgreGameKit__OGKGame__
#define __OgreGameKit__OGKGame__

// gfx
#include <Ogre.h>
#include <OgreOverlaySystem.h>

// input
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

// defines
#include "OGKStaticPluginLoader.h"

#include "OGKInputManager.h"

// camera
#include "OGKCamera.h"

// audio
#include "OGKAudio.h"

// terrain
#include "OGKTerrain.h"

#ifdef OGRE_IS_IOS
#include <OISMultiTouch.h>
class OGKGame : public Ogre::Singleton<OGKGame>, OIS::KeyListener, OIS::MultiTouchListener
#else
class OGKGame : public Ogre::Singleton<OGKGame>, OIS::KeyListener, OIS::MouseListener
#endif
{
public:
	OGKGame();
	~OGKGame();
    
    bool initOgre(Ogre::String wndTitle);
	
	bool isOgreToBeShutDown()const{return m_bShutDownOgre;}
    
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
    
    bool renderOneFrame();
    bool renderOneFrame(double timeSinceLastFrame);
    void setup();
    void start();
    void update(double timeSinceLastFrame);
    
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
	
	Ogre::Root*                 m_pRoot;
	Ogre::SceneManager*			m_pSceneMgr;
	Ogre::RenderWindow*			m_pRenderWnd;
	OGKCamera*                  mCamera;
	Ogre::Log*                  m_pLog;
	Ogre::Timer*				m_pTimer;
    Ogre::OverlaySystem*        m_pOverlaySystem;

#ifdef OGRE_IS_IOS
    Ogre::OrientationMode       mViewportOrientation;
#endif
    
protected:
    Ogre::String                 m_ResourcePath;
    
private:
	OGKGame(const OGKGame&);
	OGKGame& operator= (const OGKGame&);
    
    void _initInput();
    void _initResources();
    void _initOverlays();
    
    // terrain
    OGKTerrain *mTerrain;
    
    // overlays
    Ogre::Overlay*              m_pOverlay;
    Ogre::OverlayContainer*     m_pOverlayContainer;
    Ogre::OverlayElement*       m_pOverlayPanel;
    Ogre::OverlayElement*       m_pFPS;
    
	double                      m_TimeSinceLastFrame;
	double                      m_StartTime;
	bool                        m_bShutDownOgre;
	Ogre::Vector3				m_TranslateVector;
	Ogre::Real                  m_MoveSpeed;
	Ogre::Degree				m_RotateSpeed;
	float                       m_MoveScale;
	Ogre::Degree				m_RotScale;

    Ogre::Entity*               mPlayer;
    
#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader    m_StaticPluginLoader;
#endif
};

#endif
