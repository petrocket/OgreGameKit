//
//  OGKBadge.h
//  OgreGameKit
//
//  Created by Alex on 7/3/14.
//
//

#ifndef __OgreGameKit__OGKBadge__
#define __OgreGameKit__OGKBadge__

#include <Ogre.h>

namespace Gorilla {
    class Caption;
    class Rectangle;
    class Silverback;
    class ScreenRenderable;
}

class OGKBadge
{
public:
    OGKBadge(Ogre::SceneNode *attachToNode,
             Gorilla::Silverback *silverback,
             Ogre::Camera *camera,
             Ogre::Vector3 offset);
    ~OGKBadge();
    
    void setCaption(const Ogre::String& caption);
    
    // health must be between 0 and 1
    void setHealth(Ogre::Real health, bool animate = true);
    void setHealthBarColour(Ogre::ColourValue colour);
    void setHealthBarBorderColour(Ogre::ColourValue colour);
    
    void setVisible(bool visible);
    
    Ogre::Vector3 mOffset;
    Ogre::String mCaptionText;
    Ogre::SceneNode *mSceneNode;
    
private:
    void _init();
    
    Ogre::Camera *mCamera;
    Gorilla::Silverback *mSilverback;
    Gorilla::ScreenRenderable *mScreenRenderable;
    Gorilla::Rectangle *mHealthBarBorder;
    Gorilla::Rectangle *mHealthBar;
    Gorilla::Caption *mCaption;
};

#endif /* defined(__OgreGameKit__OGKBadge__) */
