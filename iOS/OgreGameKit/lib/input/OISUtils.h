//
//  OISUtils.h
//  OgreGameKit
//
//  Created by Alex on 7/26/14.
//
//

#ifndef OgreGameKit_OISUtils_h
#define OgreGameKit_OISUtils_h

#include <Ogre.h>
#include <OISEvents.h>
#include <OISMouse.h>
#include <OISMultiTouch.h>

OIS::MouseEvent toMouseEvent(OIS::MultiTouchEvent evt);

Ogre::Vector2 getScreenSize();

int getScreenScale();

Ogre::OrientationMode getScreenOrientation();

#endif
