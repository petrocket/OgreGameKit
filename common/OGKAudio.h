//
//  OGKGameAudio.h
//  OgreGameKit
//
//  Created by Alex on 6/6/14.
//
//

#ifndef OgreGameKit_OGKGameAudio_h
#define OgreGameKit_OGKGameAudio_h

// background music
void playBackgroundMusic(std::string filename);
void stopBackgroundMusic();

// effects
unsigned int playEffect(std::string filename);
unsigned int playEffect(std::string filename,float pitch, float pan, float gain);

#endif
