//
//  OGKAppDelgate.h
//  OgreGameKit
//
//  Created by Alex on 5/20/14.
//
//

#include "Game.h"

@interface OGKAppDelegate : NSObject <UIApplicationDelegate>
{
    id mDisplayLink;
    NSDate *mDate;
}

- (void)renderOneFrame;

@property double lastFrameTime;
@property double startTime;

@end
