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
    @autoreleasepool {
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
    }
    
    mTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0f / 60.0f)
                                              target:self
                                            selector:@selector(renderOneFrame)
                                            userInfo:nil
                                             repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:mTimer forMode:NSEventTrackingRunLoopMode];
        
}

- (void)applicationWillTerminate:(NSNotification *)notifciation
{
    if(mTimer) {
        [mTimer invalidate];
        mTimer = nil;
    }
}

- (void)renderOneFrame
{
    if(!OGKGame::getSingletonPtr()->renderOneFrame()) {
	    [NSApp terminate:self];
    }
}

@end
