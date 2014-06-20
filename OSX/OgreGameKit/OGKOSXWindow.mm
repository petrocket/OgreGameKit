//
//  OGKOSXWindow.m
//  OgreGameKit
//
//  Created by Alex on 6/20/14.
//
//

#import "OGKOSXWindow.h"
#include <Ogre.h>
// HAX
#define __glew_h__
#include <OgreOSXCocoaWindow.h>

@implementation OGKOSXWindow

Ogre::Rect getWindowBounds(Ogre::RenderWindow *renderWindow)
{
    Ogre::OSXCocoaWindow *cw = static_cast<Ogre::OSXCocoaWindow*>(renderWindow);
    NSWindow *win = cw->ogreWindow();
    
    NSScreen *screen = NSScreen.mainScreen;
    NSRect rect = screen.frame;
    
    return Ogre::Rect(win.frame.origin.x, rect.size.height - win.frame.origin.y, win.frame.size.width, win.frame.size.height);
}

@end
