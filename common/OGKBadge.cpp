//
//  OGKBadge.cpp
//  OgreGameKit
//
//  Created by Alex on 7/3/14.
//
//

#include "OGKBadge.h"
#include "Gorilla.h"

OGKBadge::OGKBadge(Ogre::SceneNode *attachToNode,
                   Gorilla::Silverback *silverback,
                   Ogre::Camera *camera,
                   Ogre::Vector3 offset) :
    mCamera(camera),
    mSceneNode(attachToNode),
    mScreenRenderable(NULL),
    mSilverback(silverback),
    mOffset(offset),
    mCaptionText("")
{
    _init();
}

OGKBadge::~OGKBadge()
{
    
}

// health must be between 0 and 1
void OGKBadge::setHealth(Ogre::Real health, bool animate)
{
    if(animate) {
        // @TODO
    }
    
    mHealthBar->width(MAX(0.0,MIN(1.0,health)) * 100.0);
}

void OGKBadge::setHealthBarColour(Ogre::ColourValue colour)
{
    mHealthBar->background_colour(colour);
}

void OGKBadge::setHealthBarBorderColour(Ogre::ColourValue colour)
{
    mHealthBarBorder->border(2,colour);
}

void OGKBadge::setCaption(const Ogre::String& caption)
{
    mCaptionText = caption;
    mCaption->text(caption);
}

void OGKBadge::setVisible(bool visible)
{
    mScreenRenderable->setVisible(visible);
}

void OGKBadge::_init()
{
    mScreenRenderable = mSilverback->createScreenRenderable(Ogre::Vector2(1,1),
                                                        "default_theme");
    
    Ogre::SceneNode *sceneNode = mSceneNode->createChildSceneNode();
    sceneNode->setPosition(mOffset);
    sceneNode->setFixedYawAxis(true);
    sceneNode->setAutoTracking(true,
                               mCamera->getParentSceneNode(),
                               Ogre::Vector3::UNIT_Z);
    sceneNode->attachObject(mScreenRenderable);
    
    Gorilla::Layer *layer = mScreenRenderable->createLayer(0);

    mCaption = layer->createCaption(14, 0, 0, "HI THERE");
    
    mHealthBarBorder = layer->createRectangle(0,30,100,10);
    mHealthBarBorder->border(2,Gorilla::Colours::DarkRed);
    mHealthBarBorder->background_colour(Gorilla::Colours::Black);
    
    mHealthBar = layer->createRectangle(0,30,100,10);
    mHealthBar->background_colour(Gorilla::Colours::DarkRed);
}