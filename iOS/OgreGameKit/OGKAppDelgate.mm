//
//  OGKAppDelgate.m
//  OgreGameKit
//
//  Created by Alex on 5/20/14.
//
//

#import <QuartzCore/QuartzCore.h>

#import "OGKAppDelgate.h"
#import "Ogre.h"

@implementation OGKAppDelegate

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
    // Hide the status bar (iOS 6)
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    self.lastFrameTime = 1;
    self.startTime = 0;
    
    try {
        new Game();
        Game::getSingleton().start();
        Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
		Ogre::Root::getSingleton().clearEventTimes();
    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }
    [pool release];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    Ogre::Root::getSingleton().saveConfig();
    
    Ogre::Root::getSingleton().queueEndRendering();
    
    if (mDisplayLink) {
        [mDate release];
        mDate = nil;
        
        [mDisplayLink invalidate];
        mDisplayLink = nil;
    }
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    mDate = [[NSDate alloc] init];
    self.lastFrameTime = -[mDate timeIntervalSinceNow];
    
    mDisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(renderOneFrame)];
    [mDisplayLink setFrameInterval:self.lastFrameTime];
    [mDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    Ogre::Root::getSingleton().queueEndRendering();
    [self terminate];
}

- (void)renderOneFrame
{
    if(!Game::getSingletonPtr()->renderOneFrame()) {
	    [self terminate];
    }
}

- (void)terminate
{
    [mDate release];
    mDate = nil;
    
    [mDisplayLink invalidate];
    mDisplayLink = nil;
    
    [[UIApplication sharedApplication] performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
}


@end
