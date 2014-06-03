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

- (void)applicationDidFinishLaunching:(NSNotification *)application
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    self.lastFrameTime = 1;
    self.startTime = 0;
    
    try {
        new OGKGame();
        OGKGame::getSingleton().start();
        
        Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
        
        // Clear event times
		Ogre::Root::getSingleton().clearEventTimes();
    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }
    
    mDate = [[NSDate alloc] init];
    self.lastFrameTime = -[mDate timeIntervalSinceNow];
    
    mTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0f / 60.0f) * self.lastFrameTime
                                              target:self
                                            selector:@selector(renderOneFrame)
                                            userInfo:nil
                                             repeats:YES];
    [pool release];
}

- (void)dealloc
{
    if(mTimer) {
        [mTimer invalidate];
        mTimer = nil;
    }
    
    [super dealloc];
}

- (void)renderOneFrame
{
    if(!OGKGame::getSingletonPtr()->renderOneFrame()) {
	    [self terminate];
    }
}

- (void)terminate
{
    [mDate release];
    mDate = nil;
    
    [mDisplayLink invalidate];
    mDisplayLink = nil;
    
    if(mTimer) {
        [mTimer invalidate];
        mTimer = nil;
    }
    
    [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
}

@end
