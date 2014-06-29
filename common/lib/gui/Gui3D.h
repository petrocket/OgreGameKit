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

#ifndef Gui3D_H
#define Gui3D_H



#include <map>
#include <vector>

#include <Ogre.h>

#include "Gorilla.h"

#include "Gui3DPanelColors.h"

namespace Gui3D
{

/** \brief Internal use only. Used to keep track of all screens's layers
*/
typedef struct Gui3DScreenRenderable Gui3DScreenRenderable;

/** \brief Internal use only. Used to keep track of all screenRenderable's layers
*/
typedef struct Gui3DScreen Gui3DScreen;
    
/** \brief Internal use only. Used to keep track of all screenRenderable2D's layers
 */
typedef struct Gui3DScreenRenderable2D Gui3DScreenRenderable2D;

// TODO : Screens methods : implements something more ?

/*! class. Gui3D
    desc.
        The main object to instanciate Gui3D.
*/
class Gui3D
{
public:
    /** \brief The PanelColors gave as argument will automatically be deleted in Gui3D destructor, or
    in the setPanelColors() method
    */
    Gui3D(PanelColors*);
    
    ~Gui3D();

    /** \brief /!\ Be careful when using this. Return the main Gorilla object.
    */
    Gorilla::Silverback* getSilverback();

    // ScreenRenderables methods (2D Screens rendered in 3D)

    Gorilla::Layer* getLayer(const Ogre::String& screenRenderableName,
                             const Ogre::String& layerName);

    Gorilla::Layer* getLayer(Gorilla::ScreenRenderable* screenRenderable,
                             const Ogre::String& layerName);

    Gorilla::Layer* createLayer(Gorilla::ScreenRenderable* screenRenderable,
                                const Ogre::String& layerName);

    Gorilla::Layer* createLayer(const Ogre::String& screenRenderableName,
                                const Ogre::String& layerName);
    
    Gorilla::ScreenRenderable* getScreenRenderable(const Ogre::String& name);
    
    Gorilla::ScreenRenderable* createScreenRenderable(const Ogre::Vector2& pos, 
                                                      const Ogre::String& atlasName, 
                                                      const Ogre::String& name);

    // Screens methods (2D screen)

    Gorilla::Layer* getLayerScreen(const Ogre::String& screenName,
                                   const Ogre::String& layerName);

    Gorilla::Layer* getLayerScreen(Gorilla::Screen* screen,
                                   const Ogre::String& layerName);

    Gorilla::Layer* createLayerScreen(Gorilla::Screen* screen,
                                      const Ogre::String& layerName);

    Gorilla::Layer* createLayerScreen(const Ogre::String& screenName,
                                      const Ogre::String& layerName);

    Gorilla::Screen* createScreen(Ogre::Viewport* vp, 
                                  const Ogre::String& atlasName, const Ogre::String& name);

    Gorilla::Screen* getScreen(const Ogre::String& name);

    
    // Screens methods (2D screen renderable)
    
    Gorilla::Layer* getLayerScreenRenderable2D(const Ogre::String& screenName,
                                   const Ogre::String& layerName);
    
    Gorilla::Layer* getLayerScreenRenderable2D(Gorilla::ScreenRenderable2D* screen,
                                   const Ogre::String& layerName);
    
    Gorilla::Layer* createLayerScreenRenderable2D(Gorilla::ScreenRenderable2D* screen,
                                      const Ogre::String& layerName);
    
    Gorilla::Layer* createLayerScreenRenderable2D(const Ogre::String& screenName,
                                      const Ogre::String& layerName);
    
    Gorilla::ScreenRenderable2D* createScreenRenderable2D(Ogre::Viewport* vp,
                                  const Ogre::String& atlasName, const Ogre::String& name);
    
    Gorilla::ScreenRenderable2D* getScreenRenderable2D(const Ogre::String& name);

    
    void destroyScreenRenderable2D(const Ogre::String& screenRenderable2DName);

    void destroyScreenRenderable2D(Gorilla::ScreenRenderable2D* screenRenderable2D);
    
    // General methods

    void loadAtlas(const Ogre::String& name);

    void destroyScreenRenderable(const Ogre::String& screenRenderableName);

    void destroyScreenRenderable(Gorilla::ScreenRenderable* screenRenderable);

    void destroyScreen(const Ogre::String& screenName);

    void destroyScreen(Gorilla::Screen* screen);

    /** \brief Set the colors rules of the Gui3D. 
    The PanelColors object will automatically be destroyed by the Gui3D destructor.
    */
    void setPanelColors(PanelColors*);

    PanelColors* getPanelColors();

protected:
    Gorilla::Silverback* mSilverback;

    std::vector<Ogre::String> mAtlas;

    std::map<Ogre::String, Gui3DScreenRenderable*> mScreenRenderables;
    
    std::map<Ogre::String, Gui3DScreenRenderable2D*> mScreenRenderables2D;

    std::map<Ogre::String, Gui3DScreen*> mScreens;

    PanelColors* mPanelColors;

    bool hasAtlas(const Ogre::String& name);
};

}

#endif