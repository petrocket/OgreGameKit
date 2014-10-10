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

#ifdef USE_CVDISPLAY_LINK
#import "OgreOSXCocoaWindow.h"
#endif

@implementation OGKAppDelegate

#ifdef USE_CVDISPLAY_LINK
static Ogre::OSXCocoaWindow *ogkWindow = 0;
#endif

- (void)applicationDidFinishLaunching:(NSNotification *)application
{
    @autoreleasepool {
        try {
            new Game();
            Game::getSingleton().start();
#ifdef USE_CVDISPLAY_LINK
            Ogre::RenderWindow *rw =OGKGame::getSingleton().m_pRenderWnd;
            ogkWindow = static_cast<Ogre::OSXCocoaWindow *>(rw);
            NSOpenGLContext *ctx = ogkWindow->nsopenGLContext();
            
            // sync buffer swap with vertical refresh rate
            GLint swapInt = 1;
            [ctx setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
            
            NSOpenGLPixelFormat *fmt = ogkWindow->nsopenGLPixelFormat();
            CGLContextObj cglContext = (CGLContextObj)[ctx CGLContextObj];
            CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[fmt CGLPixelFormatObj];
            
            CVReturn ret = kCVReturnSuccess;
            
            ret = CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
            if(ret != kCVReturnSuccess) {
                std::cerr << "Failed to create display link " << std::endl;
            }
            
            // Set the renderer output callback function
            ret = CVDisplayLinkSetOutputCallback(displayLink, &OGKDisplayLinkCallback, (__bridge void*)self);
            if(ret != kCVReturnSuccess) {
                std::cerr << "Failed to set display link callback " << std::endl;
            }
            ret = CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink,
                                                              cglContext,
                                                              cglPixelFormat);
            if(ret != kCVReturnSuccess) {
                std::cerr << "Failed to set display link context" << std::endl;
            }
            
            // Activate the display link
            ret = CVDisplayLinkStart(displayLink);
            if(ret != kCVReturnSuccess) {
                std::cerr << "Failed to start display link " << std::endl;   
            }
#endif
            
            Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
            
            // Clear event times
            Ogre::Root::getSingleton().clearEventTimes();
        } catch( Ogre::Exception& e ) {
            std::cerr << "An exception has occurred: " <<
            e.getFullDescription().c_str() << std::endl;
        }
    }   

    
#ifndef USE_CVDISPLAY_LINK
    self.lastFrameStart = NSDate.date;
    
    mTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0f / 60.0f)
                                              target:self
                                            selector:@selector(renderOneFrame)
                                            userInfo:nil
                                             repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:mTimer forMode: NSDefaultRunLoopMode];
    [[NSRunLoop currentRunLoop] addTimer:mTimer forMode:NSEventTrackingRunLoopMode];
    
#endif
}

#ifdef USE_CVDISPLAY_LINK
// This is the renderer output callback function
static CVReturn OGKDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(__bridge OGKAppDelegate*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    NSOpenGLContext *ctx = ogkWindow->nsopenGLContext();
    CGLContextObj cglContext = (CGLContextObj)[ctx CGLContextObj];
    CGLLockContext(cglContext);
    Ogre::Real deltaTime = 1.0 / (outputTime->rateScalar * (Ogre::Real)outputTime->videoTimeScale / (Ogre::Real)outputTime->videoRefreshPeriod);
    
    [ctx makeCurrentContext];
    
    if(!Game::getSingletonPtr()->renderOneFrame()) {
	    [NSApp terminate:self];
    }
    
    CGLUnlockContext(cglContext);
    return kCVReturnSuccess;
}
#endif

- (void)applicationWillTerminate:(NSNotification *)notifciation
{
    if(mTimer) {
        [mTimer invalidate];
        mTimer = nil;
    }
    
#ifdef USE_CVDISPLAY_LINK
    if(displayLink) {
        CVDisplayLinkStop(displayLink);
        CVDisplayLinkRelease(displayLink);
    }
#endif
}

- (void)renderOneFrame
{
    // we have to calculate the time elapsed here because
    // calculating using OGRE timers gives incorrect results
    double elapsedTime = [self.lastFrameStart timeIntervalSinceNow] * -1000.0;
    self.lastFrameStart = NSDate.date;
    if(!Game::getSingletonPtr()->renderOneFrame(elapsedTime)) {
	    [NSApp terminate:self];
    }
}

@end
