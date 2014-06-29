/*
    Gui3D
    -------
    
    Copyright (c) 2012 Valentin Frechaud
                                                                                  
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
                                                                                  
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
                                                                                  
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


#include "Gui3D.h"

namespace Gui3D
{

using namespace std;

struct Gui3DScreen
{
    Gui3DScreen(Gorilla::Screen* screen)
        : mScreen(screen)
    {}
    Gorilla::Screen* mScreen;
    map<Ogre::String, Gorilla::Layer*> mLayers;
};


struct Gui3DScreenRenderable
{
    Gui3DScreenRenderable(Gorilla::ScreenRenderable* screenRenderable)
        : mScreenRenderable(screenRenderable)
    {}
    Gorilla::ScreenRenderable* mScreenRenderable;
    map<Ogre::String, Gorilla::Layer*> mLayers;
};
    
struct Gui3DScreenRenderable2D
{
    Gui3DScreenRenderable2D(Gorilla::ScreenRenderable2D* screenRenderable)
    : mScreenRenderable2D(screenRenderable)
    {}
    Gorilla::ScreenRenderable2D* mScreenRenderable2D;
    map<Ogre::String, Gorilla::Layer*> mLayers;
};

Gui3D::Gui3D(PanelColors* panelColors)
    : mPanelColors(NULL)
{
    mSilverback = Gorilla::Silverback::getSingletonPtr();
    if(!mSilverback) {
        mSilverback = new Gorilla::Silverback();
    }
        
    setPanelColors(panelColors);
}


Gui3D::~Gui3D()
{
    // Implicit delete of screens, screenRenderables and atlas
    delete mSilverback;
}


Gorilla::Silverback* Gui3D::getSilverback()
{
    return mSilverback;
}


//
// ScreenRenderables methods (2D Screens rendered in 3D)
//


Gorilla::Layer* Gui3D::getLayer(const Ogre::String& screenRenderableName,
                                const Ogre::String& layerName)
{
    if (mScreenRenderables[screenRenderableName] != NULL)
        return mScreenRenderables[screenRenderableName]->mLayers[layerName];
    
    return NULL;
}


Gorilla::Layer* Gui3D::getLayer(Gorilla::ScreenRenderable* screenRenderable,
                                const Ogre::String& layerName)
{
    map<Ogre::String, Gui3DScreenRenderable*>::iterator it =
        mScreenRenderables.begin();

    for (; it != mScreenRenderables.end(); ++it)
        if (it->second->mScreenRenderable == screenRenderable)
            return getLayer(it->first, layerName);

    return NULL;
}


Gorilla::Layer* Gui3D::createLayer(Gorilla::ScreenRenderable* screenRenderable,
                                   const Ogre::String& layerName)
{
    // Tests if already exists
    Gorilla::Layer* layer = getLayer(screenRenderable, layerName);
    if (layer != NULL)
        return layer;

    map<Ogre::String, Gui3DScreenRenderable*>::iterator it = 
        mScreenRenderables.begin();

    // Try to find the screenRenderable
    for (; it != mScreenRenderables.end(); ++it)
        if (it->second->mScreenRenderable == screenRenderable)
        {
            Gorilla::Layer* layer = screenRenderable->createLayer(1);
            it->second->mLayers[layerName] = layer;
            return layer;
        }

    return NULL;
}


Gorilla::Layer* Gui3D::createLayer(const Ogre::String& screenRenderableName,
                                   const Ogre::String& layerName)
{
    // Tests if already exists
    Gorilla::Layer* layer = getLayer(screenRenderableName, layerName);
    if (layer != NULL)
        return layer;

    // Try to find the screenRenderable
    Gorilla::ScreenRenderable* screenR = 
        mScreenRenderables[screenRenderableName]->mScreenRenderable;

    if (screenR != NULL)
    {
        Gorilla::Layer* layer = screenR->createLayer();

        mScreenRenderables[screenRenderableName]->mLayers[layerName] = layer;
        return layer;
    }

    return NULL;
}


Gorilla::ScreenRenderable* Gui3D::createScreenRenderable(const Ogre::Vector2& pos, 
                                                         const Ogre::String& atlasName, 
                                                         const Ogre::String& name)
{
    if (!hasAtlas(atlasName))
        loadAtlas(atlasName);

    mScreenRenderables[name] = new Gui3DScreenRenderable(
        mSilverback->createScreenRenderable(pos, atlasName));

    return mScreenRenderables[name]->mScreenRenderable;
}
    

Gorilla::ScreenRenderable* Gui3D::getScreenRenderable(const Ogre::String& name)
{
    if (mScreenRenderables[name] != NULL)
        return mScreenRenderables[name]->mScreenRenderable;

    return NULL;
}


//
// Screens methods (2D screen)
//


Gorilla::Layer* Gui3D::getLayerScreen(const Ogre::String& screenName, 
                                      const Ogre::String& layerName)
{
    if (mScreens[screenName] != 0)
        return mScreens[screenName]->mLayers[layerName];

    return NULL;
}


Gorilla::Layer* Gui3D::getLayerScreen(Gorilla::Screen* screen, 
                                      const Ogre::String& layerName)
{
    map<Ogre::String, Gui3DScreen*>::iterator it = mScreens.begin();

    for (; it != mScreens.end(); ++it)
        if (it->second->mScreen == screen)
            return getLayerScreen(it->first, layerName);

    return NULL;
}


Gorilla::Layer* Gui3D::createLayerScreen(Gorilla::Screen* screen, 
                                         const Ogre::String& layerName)
{
    // Tests if already exists
    Gorilla::Layer* layer = getLayerScreen(screen, layerName);
    if (layer != NULL)
        return layer;

    map<Ogre::String, Gui3DScreen*>::iterator it = mScreens.begin();

    // Try to find the screen
    for (; it != mScreens.end(); ++it)
        if (it->second->mScreen == screen)
        {
            Gorilla::Layer* layer = screen->createLayer(1);
            it->second->mLayers[layerName] = layer;
            return layer;
        }

    return NULL;
}


Gorilla::Layer* Gui3D::createLayerScreen(const Ogre::String& screenName, 
                                         const Ogre::String& layerName)
{
    // Tests if already exists
    Gorilla::Layer* layer = getLayerScreen(screenName, layerName);
    if (layer != NULL)
        return layer;

    // Try to find the screen
    Gorilla::Screen* screen = mScreens[screenName]->mScreen;

    if (screen != NULL)
    {
        Gorilla::Layer* layer = screen->createLayer();

        mScreens[screenName]->mLayers[layerName] = layer;
        return layer;
    }

    return NULL;
}


Gorilla::Screen* Gui3D::createScreen(Ogre::Viewport* vp, 
                                     const Ogre::String& atlasName, 
                                     const Ogre::String& name)
{
    if (!hasAtlas(atlasName))
        loadAtlas(atlasName);

    mScreens[name] = new Gui3DScreen(mSilverback->createScreen(vp, atlasName));
    return mScreens[name]->mScreen;
}


Gorilla::Screen* Gui3D::getScreen(const Ogre::String& name)
{
    if (mScreens[name] != NULL)
        return mScreens[name]->mScreen;
    return NULL;
}

//
// Screens methods (2D screen renderable)
//
Gorilla::Layer* Gui3D::getLayerScreenRenderable2D(const Ogre::String& screenName,
                                           const Ogre::String& layerName)
{
    if (mScreenRenderables2D[screenName] != 0)
        return mScreenRenderables2D[screenName]->mLayers[layerName];
    
    return NULL;
}

Gorilla::Layer* Gui3D::getLayerScreenRenderable2D(Gorilla::ScreenRenderable2D* screen,
                                           const Ogre::String& layerName)
{
    map<Ogre::String, Gui3DScreenRenderable2D*>::iterator it = mScreenRenderables2D.begin();
    
    for (; it != mScreenRenderables2D.end(); ++it)
        if (it->second->mScreenRenderable2D == screen)
            return getLayerScreenRenderable2D(it->first, layerName);
    
    return NULL;
}

Gorilla::Layer* Gui3D::createLayerScreenRenderable2D(Gorilla::ScreenRenderable2D* screen,
                                              const Ogre::String& layerName)
{
    // Tests if already exists
    Gorilla::Layer* layer = getLayerScreenRenderable2D(screen, layerName);
    if (layer != NULL)
        return layer;
    
    map<Ogre::String, Gui3DScreenRenderable2D*>::iterator it = mScreenRenderables2D.begin();
    
    // Try to find the screen
    for (; it != mScreenRenderables2D.end(); ++it)
        if (it->second->mScreenRenderable2D == screen)
        {
            Gorilla::Layer* layer = screen->createLayer(1);
            it->second->mLayers[layerName] = layer;
            return layer;
        }
    
    return NULL;
}

Gorilla::Layer* Gui3D::createLayerScreenRenderable2D(const Ogre::String& screenName,
                                              const Ogre::String& layerName)
{
    // Tests if already exists
    Gorilla::Layer* layer = getLayerScreenRenderable2D(screenName, layerName);
    if (layer != NULL)
        return layer;
    
    // Try to find the screen
    Gorilla::ScreenRenderable2D* screen = mScreenRenderables2D[screenName]->mScreenRenderable2D;
    
    if (screen != NULL)
    {
        Gorilla::Layer* layer = screen->createLayer();
        
        mScreenRenderables2D[screenName]->mLayers[layerName] = layer;
        return layer;
    }
    
    return NULL;
}
    
Gorilla::ScreenRenderable2D* Gui3D::createScreenRenderable2D(Ogre::Viewport* vp,
                                                      const Ogre::String& atlasName, const Ogre::String& name)
{
    if (!hasAtlas(atlasName))
        loadAtlas(atlasName);
    
    mScreenRenderables2D[name] = new Gui3DScreenRenderable2D(mSilverback->createScreenRenderable2D(vp, atlasName));
    return mScreenRenderables2D[name]->mScreenRenderable2D;
}

Gorilla::ScreenRenderable2D* Gui3D::getScreenRenderable2D(const Ogre::String& name)
{
    if (mScreenRenderables2D[name] != NULL)
        return mScreenRenderables2D[name]->mScreenRenderable2D;
    return NULL;
}


//
// General methods
//


bool Gui3D::hasAtlas(const Ogre::String& name)
{
    for (size_t i=0; i < mAtlas.size(); i++)
        if (mAtlas[i] == name)
            return true;
    return false;
}


void Gui3D::loadAtlas(const Ogre::String& name)
{
    mSilverback->loadAtlas(name);
    mAtlas.push_back(name);
}


void Gui3D::destroyScreenRenderable(const Ogre::String& screenRenderableName)
{
    // Will implicitly destroy its layers
    mSilverback->destroyScreenRenderable(mScreenRenderables[screenRenderableName]->mScreenRenderable);
    mScreenRenderables.erase(screenRenderableName);
}


void Gui3D::destroyScreenRenderable(Gorilla::ScreenRenderable* screenRenderable)
{
    map<Ogre::String, Gui3DScreenRenderable*>::iterator it =
        mScreenRenderables.begin();

    for (; it != mScreenRenderables.end(); ++it)
        if (it->second->mScreenRenderable == screenRenderable)
        {
            destroyScreenRenderable(it->first);
            break;
        }
}
    
    void Gui3D::destroyScreenRenderable2D(const Ogre::String& screenName)
    {
        // Will implicitly destroy its layers
        mSilverback->destroyScreenRenderable2D(mScreenRenderables2D[screenName]->mScreenRenderable2D);
        mScreens.erase(screenName);
    }


void Gui3D::destroyScreenRenderable2D(Gorilla::ScreenRenderable2D* screenRenderable2D)
{
    map<Ogre::String, Gui3DScreenRenderable2D*>::iterator it =
    mScreenRenderables2D.begin();
    
    for (; it != mScreenRenderables2D.end(); ++it)
        if (it->second->mScreenRenderable2D == screenRenderable2D)
        {
            destroyScreenRenderable2D(it->first);
            break;
        }
}


void Gui3D::destroyScreen(const Ogre::String& screenName)
{
    // Will implicitly destroy its layers
    mSilverback->destroyScreen(mScreens[screenName]->mScreen);
    mScreens.erase(screenName);
}


void Gui3D::destroyScreen(Gorilla::Screen* screen)
{
    map<Ogre::String, Gui3DScreen*>::iterator it =
        mScreens.begin();

    for (; it != mScreens.end(); ++it)
        if (it->second->mScreen == screen)
        {
            destroyScreen(it->first);
            break;
        }
}


void Gui3D::setPanelColors(PanelColors* panelColors)
{
    mPanelColors = panelColors;
}


PanelColors* Gui3D::getPanelColors()
{
    return mPanelColors;
}

}