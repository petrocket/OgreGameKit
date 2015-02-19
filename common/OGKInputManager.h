//
//  OGKInputManager.h
//  OgreGameKit
//
//  Created by Alex on 6/19/14.
//
//

#ifndef __OgreGameKit__OGKInputManager__
#define __OgreGameKit__OGKInputManager__

#include <Ogre.h>
#include "OGKStaticPluginLoader.h"

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISJoyStick.h>

#ifdef OGRE_IS_IOS
#include <OISMultiTouch.h>
#include <OISUtils.h>
    class OGKInputManager : public OIS::KeyListener, public OIS::MultiTouchListener, public OIS::JoyStickListener {
#else
    class OGKInputManager : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener {
#endif
public:
    virtual ~OGKInputManager( void );
    
    void initialise( Ogre::RenderWindow *renderWindow, bool showMouseCursor = false );
    void reset();
    void capture( void );
    
    void addKeyListener( OIS::KeyListener *keyListener, const std::string& instanceName );
#ifdef OGRE_IS_IOS
    void addMultiTouchListener( OIS::MultiTouchListener *mouseListener, const std::string& instanceName );
#else
    void addMouseListener( OIS::MouseListener *mouseListener, const std::string& instanceName );
#endif
    
    void addJoystickListener( OIS::JoyStickListener *joystickListener, const std::string& instanceName );
    
    void removeAllListeners( void );

    void removeKeyListener( const std::string& instanceName );
    void removeKeyListener( OIS::KeyListener *keyListener );
    void removeAllKeyListeners( void );

    void removeMouseListener( const std::string& instanceName );
#ifdef OGRE_IS_IOS
    void removeMultiTouchListener( OIS::MultiTouchListener *mouseListener );
#else
    void removeMouseListener( OIS::MouseListener *mouseListener );
#endif
    void removeAllMouseListeners( void );
    void removeJoystickListener( OIS::JoyStickListener *joystickListener );
    void removeJoystickListener( const std::string& instanceName );
    void removeAllJoystickListeners( void );
    
    void setWindowExtents( int width, int height );
    
#ifdef OGRE_IS_IOS
    OIS::MultiTouch* getMouse( void );
#else
    OIS::Mouse*    getMouse( void );
        void setMouseVisible(bool visible);
#endif
    OIS::Keyboard* getKeyboard( void );
    OIS::JoyStick* getJoystick( unsigned int index );
    
    int getNumOfJoysticks( void );
    
    static OGKInputManager* getSingletonPtr( void );
private:
    OGKInputManager( void );
    OGKInputManager( const OGKInputManager& ) { }
    OGKInputManager & operator = ( const OGKInputManager& );
    
    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );
    
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
    
    bool povMoved( const OIS::JoyStickEvent &e, int pov );
    bool axisMoved( const OIS::JoyStickEvent &e, int axis );
    bool sliderMoved( const OIS::JoyStickEvent &e, int sliderID );
    bool buttonPressed( const OIS::JoyStickEvent &e, int button );
    bool buttonReleased( const OIS::JoyStickEvent &e, int button );
    
    OIS::InputManager *mInputSystem;
#ifndef OGRE_IS_IOS
    OIS::Mouse        *mMouse;
#else
    OIS::MultiTouch   *mMouse;
#endif
    OIS::Keyboard     *mKeyboard;
    
    std::vector<OIS::JoyStick*> mJoysticks;
    std::vector<OIS::JoyStick*>::iterator itJoystick;
    std::vector<OIS::JoyStick*>::iterator itJoystickEnd;
    
    std::map<std::string, OIS::KeyListener*> mKeyListeners;
#ifndef OGRE_IS_IOS
    std::map<std::string, OIS::MouseListener*> mMouseListeners;
#else
    std::map<std::string, OIS::MultiTouchListener*> mMouseListeners;
#endif
    std::map<std::string, OIS::JoyStickListener*> mJoystickListeners;
    
    std::map<std::string, OIS::KeyListener*>::iterator itKeyListener;
#ifndef OGRE_IS_IOS
    std::map<std::string, OIS::MouseListener*>::iterator itMouseListener;
#else
    std::map<std::string, OIS::MultiTouchListener*>::iterator itMouseListener;
#endif
    std::map<std::string, OIS::JoyStickListener*>::iterator itJoystickListener;
    
    std::map<std::string, OIS::KeyListener*>::iterator itKeyListenerEnd;
#ifndef OGRE_IS_IOS
    std::map<std::string, OIS::MouseListener*>::iterator itMouseListenerEnd;
#else
    std::map<std::string, OIS::MultiTouchListener*>::iterator itMouseListenerEnd;
#endif
    std::map<std::string, OIS::JoyStickListener*>::iterator itJoystickListenerEnd;
    
    static OGKInputManager *mInputManager;
        
    Ogre::RenderWindow *mRenderWindow;
        
#ifdef OGRE_IS_IOS
        OIS::MultiTouchState _fixedTouchState(const OIS:: MultiTouchEvent &e);
#endif
};

#endif /* defined(__OgreGameKit__OGKInputManager__) */
