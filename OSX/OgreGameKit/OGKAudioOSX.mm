//
//  OGKAudioiOS.m
//  OgreGameKit
//
//  Created by Alex on 6/6/14.
//
//

#import "OGKAudioOSX.h"
#import "SimpleAudioEngine.h"

#include <string>

@implementation OGKAudioOSX

void playBackgroundMusic(std::string filename)
{
    // hack for now
    [SimpleAudioEngine sharedEngine].backgroundMusicVolume = 0.25;
    [[SimpleAudioEngine sharedEngine] playBackgroundMusic:[NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]]];
}

void stopBackgroundMusic()
{
    [[SimpleAudioEngine sharedEngine] stopBackgroundMusic];
}

unsigned int playEffect(std::string filename)
{
    return [[SimpleAudioEngine sharedEngine] playEffect:[NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]]];
}

unsigned int playEffect(std::string filename,float pitch, float pan, float gain)
{
    return [[SimpleAudioEngine sharedEngine] playEffect:[NSString stringWithCString:filename.c_str() encoding:[NSString defaultCStringEncoding]] pitch:pitch pan:pan gain:gain];
}

@end
