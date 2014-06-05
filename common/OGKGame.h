//
//  OGKGame.h
//  OgreGameKit
//
//  Created by Alex on 5/20/14.
//
//

#ifndef __OgreGameKit__OGKGame__
#define __OgreGameKit__OGKGame__

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlaySystem.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

// input
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

// terrain
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>

#include "OGKStaticPluginLoader.h"

#ifdef OGRE_IS_IOS
#   include <OISMultiTouch.h>
#endif

#ifdef INCLUDE_RTSHADER_SYSTEM
#   include "OGKShaderGeneratorListener.h"
#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

#ifdef OGRE_IS_IOS
class OGKGame : public Ogre::Singleton<OGKGame>, OIS::KeyListener, OIS::MultiTouchListener
#else
class OGKGame : public Ogre::Singleton<OGKGame>, OIS::KeyListener, OIS::MouseListener
#endif
{
public:
	OGKGame();
	~OGKGame();
    
#ifdef OGRE_IS_IOS
    bool initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MultiTouchListener *pMouseListener = 0);
#else
	bool initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
#endif
	
	void moveCamera();
	void getInput();
    
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
	Ogre::Camera*				m_pCamera;
	Ogre::Viewport*				m_pViewport;
	Ogre::Log*                  m_pLog;
	Ogre::Timer*				m_pTimer;
    Ogre::OverlaySystem*        m_pOverlaySystem;
    Ogre::Overlay*              m_pOverlay;
    Ogre::OverlayContainer*     m_pOverlayContainer;
    Ogre::OverlayElement*       m_pOverlayPanel;
    Ogre::OverlayElement*       m_pFPS;
	
	OIS::InputManager*			m_pInputMgr;
	OIS::Keyboard*				m_pKeyboard;
#ifdef OGRE_IS_IOS
	OIS::MultiTouch*			m_pMouse;
#else
	OIS::Mouse*					m_pMouse;
#endif
    
protected:
    Ogre::String                 m_ResourcePath;
    Ogre::TerrainGroup          *mTerrainGroup;
    Ogre::TerrainGlobalOptions  *mTerrainGlobals;
    
    void configureTerrainDefaults(Ogre::Light *light);
    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain *t);
    bool mTerrainsImported;
    
private:
	OGKGame(const OGKGame&);
	OGKGame& operator= (const OGKGame&);
    
    void _initOverlays();
    
    bool initialiseRTShaderSystem(Ogre::SceneManager* sceneMgr);
    void destroyRTShaderSystem();
    
#ifdef INCLUDE_RTSHADER_SYSTEM
    Ogre::RTShader::ShaderGenerator*			mShaderGenerator;			// The Shader generator instance.
    OGKShaderGeneratorListener*	mMaterialMgrListener;		// Shader generator material manager listener.
#endif // INCLUDE_RTSHADER_SYSTEM
    
	double m_TimeSinceLastFrame;
	double m_StartTime;
    
    Ogre::FrameEvent            m_FrameEvent;
	int                         m_iNumScreenShots;
    
	bool                        m_bShutDownOgre;
	
	Ogre::Vector3				m_TranslateVector;
	Ogre::Real                  m_MoveSpeed;
	Ogre::Degree				m_RotateSpeed;
	float                       m_MoveScale;
	Ogre::Degree				m_RotScale;
#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader    m_StaticPluginLoader;
#endif
};

#endif /* defined(__OgreGameKit__OGKGame__) */
