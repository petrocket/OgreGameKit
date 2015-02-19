//
//  OGKOGKInputManager.cpp
//  OgreGameKit
//
//  Created by Alex on 6/19/14.
//
//

#include "OGKInputManager.h"

#ifdef OIS_APPLE_PLATFORM
#include <Carbon/Carbon.h>
#include "OGKOSXWindowInterface.h"
#endif

#include "OGKGame.h"

#ifdef OGRE_IS_IOS
#include "OISUtils.h"
#endif

OGKInputManager *OGKInputManager::mInputManager;

OGKInputManager::OGKInputManager( void ) :
mMouse( 0 ),
mKeyboard( 0 ),
mInputSystem( 0 ) {
}

OGKInputManager::~OGKInputManager( void ) {
    if( mInputSystem ) {
        if( mMouse ) {
            mInputSystem->destroyInputObject( mMouse );
            mMouse = 0;
        }
        
        if( mKeyboard ) {
            mInputSystem->destroyInputObject( mKeyboard );
            mKeyboard = 0;
        }
        
        if( mJoysticks.size() > 0 ) {
            itJoystick    = mJoysticks.begin();
            itJoystickEnd = mJoysticks.end();
            for(; itJoystick != itJoystickEnd; ++itJoystick ) {
                mInputSystem->destroyInputObject( *itJoystick );
            }
            
            mJoysticks.clear();
        }
        
        mInputSystem->destroyInputSystem( mInputSystem );
        mInputSystem = 0;
        
        // Clear Listeners
        mKeyListeners.clear();
        mMouseListeners.clear();
        mJoystickListeners.clear();
    }
}

void OGKInputManager::initialise( Ogre::RenderWindow *renderWindow, bool showMouseCursor ) {
    if( !mInputSystem ) {
        mRenderWindow = renderWindow;
        
        // Setup basic variables
        OIS::ParamList paramList;
        unsigned long hWnd = 0;
        renderWindow->getCustomAttribute("WINDOW", &hWnd);

        paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
        
        // Fill parameter list
        if(showMouseCursor) {
#if defined OIS_WIN32_PLATFORM
            paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
            paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
            paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
            paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
            paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
            paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
            paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
            paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
        }
        // Create inputsystem
        mInputSystem = OIS::InputManager::createInputSystem( paramList );
        
#ifdef OGRE_IS_IOS
        mMouse = static_cast<OIS::MultiTouch*>( mInputSystem->createInputObject(OIS::OISMultiTouch, true));
        mMouse->setEventCallback( this );

        Ogre::Vector2 screenSize = getScreenSize();
        
        // Set mouse region
        this->setWindowExtents( screenSize.x, screenSize.y );
#else
        mKeyboard = static_cast<OIS::Keyboard*>( mInputSystem->createInputObject( OIS::OISKeyboard, true ) );
        mKeyboard->setEventCallback( this );
        
        mMouse = static_cast<OIS::Mouse*>( mInputSystem->createInputObject( OIS::OISMouse, true ) );
        mMouse->setEventCallback( this );
        
        // Get window size
        unsigned int width, height, depth;
        int left, top;
        renderWindow->getMetrics( width, height, depth, left, top );
        
        // Set mouse region
        this->setWindowExtents( width, height );
#endif

        
#ifdef OIS_APPLE_PLATFORM
        if(showMouseCursor) {
            CGDisplayShowCursor(kCGDirectMainDisplay);
            CGAssociateMouseAndMouseCursorPosition(TRUE);
        }
        else {
            // fix mouse in wrong place
            Ogre::Rect r = getWindowBounds(renderWindow);
            CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, CGPointMake(r.left + r.right / 2, r.top - r.bottom / 2));
        }
#endif
        // If possible create all joysticks in buffered mode
        /*
        if( mInputSystem->numJoySticks() > 0 ) {
            mJoysticks.resize( mInputSystem->numJoySticks() );
            
            itJoystick    = mJoysticks.begin();
            itJoystickEnd = mJoysticks.end();
            for(; itJoystick != itJoystickEnd; ++itJoystick ) {
                (*itJoystick) = static_cast<OIS::JoyStick*>( mInputSystem->createInputObject( OIS::OISJoyStick, true ) );
                (*itJoystick)->setEventCallback( this );
            }
        }
         */
    }
}

void OGKInputManager::capture( void ) {
    // Need to capture / update each device every frame
    if( mMouse ) {
        mMouse->capture();
    }
    
    if( mKeyboard ) {
        mKeyboard->capture();
    }
    
    if( mJoysticks.size() > 0 ) {
        itJoystick    = mJoysticks.begin();
        itJoystickEnd = mJoysticks.end();
        for(; itJoystick != itJoystickEnd; ++itJoystick ) {
            (*itJoystick)->capture();
        }
    }
}

void OGKInputManager::reset()
{
    OIS::InputManager::destroyInputSystem(mInputSystem);
    mInputSystem = NULL;
    initialise(mRenderWindow, false);
}

void OGKInputManager::addKeyListener( OIS::KeyListener *keyListener, const std::string& instanceName ) {
    if( mKeyboard ) {
        // Check for duplicate items
        itKeyListener = mKeyListeners.find( instanceName );
        if( itKeyListener == mKeyListeners.end() ) {
            mKeyListeners[ instanceName ] = keyListener;
        }
        else {
            // Duplicate Item
        }
    }
}

#ifdef OGRE_IS_IOS
void OGKInputManager::addMultiTouchListener( OIS::MultiTouchListener *mouseListener, const std::string& instanceName )
{
    if( mMouse ) {
        // Check for duplicate items
        itMouseListener = mMouseListeners.find( instanceName );
        if( itMouseListener == mMouseListeners.end() ) {
            mMouseListeners[ instanceName ] = mouseListener;
        }
        else {
            // Duplicate Item
        }
    }
}
#else
void OGKInputManager::addMouseListener( OIS::MouseListener *mouseListener, const std::string& instanceName ) {
    if( mMouse ) {
        // Check for duplicate items
        itMouseListener = mMouseListeners.find( instanceName );
        if( itMouseListener == mMouseListeners.end() ) {
            mMouseListeners[ instanceName ] = mouseListener;
        }
        else {
            // Duplicate Item
        }
    }
}
#endif

void OGKInputManager::addJoystickListener( OIS::JoyStickListener *joystickListener, const std::string& instanceName ) {
    if( mJoysticks.size() > 0 ) {
        // Check for duplicate items
        itJoystickListener = mJoystickListeners.find( instanceName );
        if( itJoystickListener == mJoystickListeners.end() ) {
            mJoystickListeners[ instanceName ] = joystickListener;
        }
        else {
            // Duplicate Item
        }
    }
}

void OGKInputManager::removeKeyListener( const std::string& instanceName ) {
    // Check if item exists
    itKeyListener = mKeyListeners.find( instanceName );
    if( itKeyListener != mKeyListeners.end() ) {
        mKeyListeners.erase( itKeyListener );
    }
    else {
        // Doesn't Exist
    }
}

void OGKInputManager::removeMouseListener( const std::string& instanceName ) {
    // Check if item exists
    itMouseListener = mMouseListeners.find( instanceName );
    if( itMouseListener != mMouseListeners.end() ) {
        mMouseListeners.erase( itMouseListener );
    }
    else {
        // Doesn't Exist
    }
}

void OGKInputManager::removeJoystickListener( const std::string& instanceName ) {
    // Check if item exists
    itJoystickListener = mJoystickListeners.find( instanceName );
    if( itJoystickListener != mJoystickListeners.end() ) {
        mJoystickListeners.erase( itJoystickListener );
    }
    else {
        // Doesn't Exist
    }
}

void OGKInputManager::removeKeyListener( OIS::KeyListener *keyListener ) {
    itKeyListener    = mKeyListeners.begin();
    itKeyListenerEnd = mKeyListeners.end();
    for(; itKeyListener != itKeyListenerEnd; ++itKeyListener ) {
        if( itKeyListener->second == keyListener ) {
            mKeyListeners.erase( itKeyListener );
            break;
        }
    }
}

#ifdef OGRE_IS_IOS
void OGKInputManager::removeMultiTouchListener( OIS::MultiTouchListener *mouseListener )
{
    itMouseListener    = mMouseListeners.begin();
    itMouseListenerEnd = mMouseListeners.end();
    for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) {
        if( itMouseListener->second == mouseListener ) {
            mMouseListeners.erase( itMouseListener );
            break;
        }
    }
}
#else
void OGKInputManager::removeMouseListener( OIS::MouseListener *mouseListener )
{
    itMouseListener    = mMouseListeners.begin();
    itMouseListenerEnd = mMouseListeners.end();
    for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) {
        if( itMouseListener->second == mouseListener ) {
            mMouseListeners.erase( itMouseListener );
            break;
        }
    }
}
#endif


void OGKInputManager::removeJoystickListener( OIS::JoyStickListener *joystickListener ) {
    itJoystickListener    = mJoystickListeners.begin();
    itJoystickListenerEnd = mJoystickListeners.end();
    for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener ) {
        if( itJoystickListener->second == joystickListener ) {
            mJoystickListeners.erase( itJoystickListener );
            break;
        }
    }
}

void OGKInputManager::removeAllListeners( void ) {
    mKeyListeners.clear();
    mMouseListeners.clear();
    mJoystickListeners.clear();
}

void OGKInputManager::removeAllKeyListeners( void ) {
    mKeyListeners.clear();
}

void OGKInputManager::removeAllMouseListeners( void ) {
    mMouseListeners.clear();
}

void OGKInputManager::removeAllJoystickListeners( void ) {
    mJoystickListeners.clear();
}

void OGKInputManager::setWindowExtents( int width, int height ) {
    // Set mouse region (if window resizes, we should alter this to reflect as well)
#ifndef OGRE_IS_IOS
    const OIS::MouseState &mouseState = mMouse->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
#else
    std::vector<OIS::MultiTouchState> states = mMouse->getMultiTouchStates();
    for(int i = 0; i < states.size(); i++) {
        const OIS::MultiTouchState &multiTouchState = states[i];
        multiTouchState.width = width;
        multiTouchState.height = height;
    }
#endif
    Ogre::LogManager::getSingletonPtr()->getDefaultLog()->logMessage("Window Extents w: " + Ogre::StringConverter::toString(width) +
                                                                     " h: " + Ogre::StringConverter::toString(height));
}

#ifdef OGRE_IS_IOS
OIS::MultiTouch* OGKInputManager::getMouse( void )
#else
OIS::Mouse* OGKInputManager::getMouse( void )
#endif
{
    return mMouse;
}
#ifndef OGRE_IS_IOS
void OGKInputManager::setMouseVisible(bool visible)
{
    OIS::InputManager::destroyInputSystem(mInputSystem);
    mInputSystem = NULL;
    initialise(mRenderWindow, visible);
}
#endif
OIS::Keyboard* OGKInputManager::getKeyboard( void ) {
    return mKeyboard;
}

OIS::JoyStick* OGKInputManager::getJoystick( unsigned int index ) {
    // Make sure it's a valid index
    if( index < mJoysticks.size() ) {
        return mJoysticks[ index ];
    }
    
    return 0;
}

int OGKInputManager::getNumOfJoysticks( void ) {
    // Cast to keep compiler happy ^^
    return (int) mJoysticks.size();
}

bool OGKInputManager::keyPressed( const OIS::KeyEvent &e ) {
    itKeyListener    = mKeyListeners.begin();
    itKeyListenerEnd = mKeyListeners.end();
    for(; itKeyListener != itKeyListenerEnd; ++itKeyListener ) {
        if(!itKeyListener->second->keyPressed( e ))
			break;
    }
    
    return true;
}

bool OGKInputManager::keyReleased( const OIS::KeyEvent &e ) {
    itKeyListener    = mKeyListeners.begin();
    itKeyListenerEnd = mKeyListeners.end();
    for(; itKeyListener != itKeyListenerEnd; ++itKeyListener ) {
        if(!itKeyListener->second->keyReleased( e ))
			break;
    }
    
    return true;
}

#ifdef OGRE_IS_IOS

bool OGKInputManager::touchMoved(const OIS::MultiTouchEvent &e)
{
    itMouseListener    = mMouseListeners.begin();
    itMouseListenerEnd = mMouseListeners.end();
    
    OIS::MultiTouchEvent m = OIS::MultiTouchEvent(NULL,_fixedTouchState(e));

    for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) {
        if(!itMouseListener->second->touchMoved( m ))
			break;
    }
	return true;
}

bool OGKInputManager::touchPressed(const OIS:: MultiTouchEvent &e)
{
    itMouseListener    = mMouseListeners.begin();
    itMouseListenerEnd = mMouseListeners.end();
    
    OIS::MultiTouchEvent m = OIS::MultiTouchEvent(NULL,_fixedTouchState(e));
    
    for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) {
        if(!itMouseListener->second->touchPressed( m ))
			break;
    }
	return true;
}

bool OGKInputManager::touchReleased(const OIS:: MultiTouchEvent &e)
{
    itMouseListener    = mMouseListeners.begin();
    itMouseListenerEnd = mMouseListeners.end();
    
    OIS::MultiTouchEvent m = OIS::MultiTouchEvent(NULL,_fixedTouchState(e));
    
    for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) {
        if(!itMouseListener->second->touchReleased( m ))
			break;
    }
	return true;
}

bool OGKInputManager::touchCancelled(const OIS:: MultiTouchEvent &e)
{
    itMouseListener    = mMouseListeners.begin();
    itMouseListenerEnd = mMouseListeners.end();
    
    OIS::MultiTouchEvent m = OIS::MultiTouchEvent(NULL,_fixedTouchState(e));
    
    for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) {
        if(!itMouseListener->second->touchCancelled( m ))
			break;
    }
	return true;
}

#else

bool OGKInputManager::mouseMoved( const OIS::MouseEvent &e ) {
    itMouseListener    = mMouseListeners.begin();
    itMouseListenerEnd = mMouseListeners.end();
    for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) {
        if(!itMouseListener->second->mouseMoved( e ))
			break;
    }
    
    return true;
}

bool OGKInputManager::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
    itMouseListener    = mMouseListeners.begin();
    itMouseListenerEnd = mMouseListeners.end();
    for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) {
        if(!itMouseListener->second->mousePressed( e, id ))
			break;
    }
    
    return true;
}

bool OGKInputManager::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
    itMouseListener    = mMouseListeners.begin();
    itMouseListenerEnd = mMouseListeners.end();
    for(; itMouseListener != mMouseListeners.end(); ++itMouseListener ) {
        if(!itMouseListener->second->mouseReleased( e, id ))
			break;
    }
    
    return true;
}
#endif

bool OGKInputManager::povMoved( const OIS::JoyStickEvent &e, int pov )
{
    itJoystickListener    = mJoystickListeners.begin();
    itJoystickListenerEnd = mJoystickListeners.end();
    for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener ) {
        if(!itJoystickListener->second->povMoved( e, pov ))
			break;
    }
    
    return true;
}

bool OGKInputManager::axisMoved( const OIS::JoyStickEvent &e, int axis )
{
    itJoystickListener    = mJoystickListeners.begin();
    itJoystickListenerEnd = mJoystickListeners.end();
    for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener ) {
        if(!itJoystickListener->second->axisMoved( e, axis ))
			break;
    }
    
    return true;
}

bool OGKInputManager::sliderMoved( const OIS::JoyStickEvent &e, int sliderID )
{
    itJoystickListener    = mJoystickListeners.begin();
    itJoystickListenerEnd = mJoystickListeners.end();
    for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener ) {
        if(!itJoystickListener->second->sliderMoved( e, sliderID ))
			break;
    }
    
    return true;
}

bool OGKInputManager::buttonPressed( const OIS::JoyStickEvent &e, int button )
{
    itJoystickListener    = mJoystickListeners.begin();
    itJoystickListenerEnd = mJoystickListeners.end();
    for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener ) {
        if(!itJoystickListener->second->buttonPressed( e, button ))
			break;
    }
    
    return true;
}

bool OGKInputManager::buttonReleased( const OIS::JoyStickEvent &e, int button )
{
    itJoystickListener    = mJoystickListeners.begin();
    itJoystickListenerEnd = mJoystickListeners.end();
    for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener )
    {
        if(!itJoystickListener->second->buttonReleased( e, button ))
			break;
    }
    
    return true;
}


OGKInputManager* OGKInputManager::getSingletonPtr( void )
{
    if( !mInputManager ) {
        mInputManager = new OGKInputManager();
    }
    
    return mInputManager;
}

#ifdef OGRE_IS_IOS
OIS::MultiTouchState OGKInputManager::_fixedTouchState(const OIS:: MultiTouchEvent &e)
{
    OIS::MultiTouchState s;
    Ogre::Vector2 screenSize = getScreenSize();
    s.X.abs = e.state.X.abs * getScreenScale();
    s.Y.abs = e.state.Y.abs * getScreenScale();
    s.X.rel = e.state.X.rel;
    s.Y.rel = e.state.Y.rel;
    s.width = screenSize.x;
    s.height = screenSize.y;
    
    if(getScreenOrientation() == Ogre::OR_PORTRAIT) {
        
    }
    else {
        // iOS 7 locked the screen dimensions, iOS 8+ gives dimesions based on orientation
        if (s.height > s.width) {
            // this is for iOS 7
            s.width = screenSize.y;
            s.height = screenSize.x;
            
            s.X.abs = e.state.Y.abs * getScreenScale();
            s.Y.abs = e.state.X.abs * getScreenScale();
            
            if(getScreenOrientation() == Ogre::OR_LANDSCAPELEFT) {
                s.X.abs = s.width - s.X.abs;
            }
            else if(getScreenOrientation() == Ogre::OR_LANDSCAPERIGHT) {
                s.Y.abs = s.height - s.Y.abs;
            }
        }
    }
    
    OGKLOG(Ogre::StringConverter::toString(s.X.abs) + " " + Ogre::StringConverter::toString(s.Y.abs));
    
    return s;
}
#endif