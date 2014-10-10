//
//  OGKAppDelgate.h
//  OgreGameKit
//
//  Created by Alex on 5/20/14.
//
//

#include "Game.h"

#ifdef __OBJC__

@interface OGKAppDelegate : NSObject <NSApplicationDelegate>
{
    CVDisplayLinkRef displayLink;
    NSTimer *mTimer;
}

@property (nonatomic, strong) NSDate *lastFrameStart;

- (void)renderOneFrame;

@end

#if __LP64__
static id mAppDelegate;
#endif

#endif
