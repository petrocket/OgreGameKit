//
//  OISUtils.mm
//  OgreGameKit
//
//  Created by Alex on 7/26/14.
//
//

#include "OISUtils.h"
#include "OGKGame.h"

OIS::MouseEvent toMouseEvent(OIS::MultiTouchEvent evt) {
    OIS::MouseState state;
    state.Z = evt.state.Z;
    CGSize size = [UIScreen mainScreen].bounds.size;

    if(UIInterfaceOrientationIsPortrait([UIApplication sharedApplication].statusBarOrientation)) {
        state.X = evt.state.X;
        state.Y = evt.state.Y;
        state.width = size.width;
        state.height = size.height;
    }
    else {
        state.width = size.height;
        state.height = size.width;
        state.X = evt.state.Y;
        state.Y = evt.state.X;
        if([UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationLandscapeLeft) {
            state.X.abs = state.width - state.X.abs;
            state.Y.abs = state.height - state.Y.abs;
        }
    }
    OIS::MouseEvent m = OIS::MouseEvent(NULL, state);

    OGKLOG("toMouseEvent x: " + Ogre::StringConverter::toString(m.state.X.abs) + " y: " + Ogre::StringConverter::toString(m.state.Y.abs) + " width: " + Ogre::StringConverter::toString(m.state.width) + " height: " + Ogre::StringConverter::toString(m.state.height));
    
    return m;
}

Ogre::Vector2 getScreenSize()
{
    CGSize size = [UIScreen mainScreen].bounds.size;
    
    size.width *= getScreenScale();
    size.height *= getScreenScale();
    
//    OGKLOG("Screen bounds w: " + Ogre::StringConverter::toString(size.width) + " h: " + Ogre::StringConverter::toString(size.height));
    if(UIInterfaceOrientationIsPortrait([UIApplication sharedApplication].statusBarOrientation)) {
        return Ogre::Vector2(size.width,size.height);
    }
    else {
        return Ogre::Vector2(size.width,size.height);
    }
}

int getScreenScale()
{
    return (int)[[UIScreen mainScreen] scale];
}

Ogre::OrientationMode getScreenOrientation()
{
    switch([UIApplication sharedApplication].statusBarOrientation) {
        case UIInterfaceOrientationLandscapeRight:
            return Ogre::OR_LANDSCAPERIGHT;
        case UIInterfaceOrientationPortrait:
            return Ogre::OR_PORTRAIT;
        case UIInterfaceOrientationPortraitUpsideDown:
            return Ogre::OR_PORTRAIT;
        case UIInterfaceOrientationLandscapeLeft:
        default:
            return Ogre::OR_LANDSCAPELEFT;
    }
}

