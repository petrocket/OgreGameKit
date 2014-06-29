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

#ifndef Gui3DPanel_H
#define Gui3DPanel_H



#include "Gorilla.h"

#include "Gui3D.h"
#include "Gui3DPanelContainer.h"

namespace Gui3D
{

//
class Panel : public PanelContainer
{
public:
    Panel(Gui3D* gui, 
          Ogre::SceneManager* sceneMgr, 
          const Ogre::Vector2& size,
          Ogre::Real distanceFromPanelToInteractWith,
          const Ogre::String& atlasName,
          const Ogre::String& name);

    ~Panel();
    
    /** \brief Set the distance the user can interact with the panel
    */
    void setDistanceFromPanelToInteractWith(Ogre::Real distanceFromPanelToInteractWith);

    /** \brief Send a ray from a part of the screen to this panel to
    actualize the internal mouse position on the panel.
    Return if the ray intersects with the panel. 
    */
    bool injectMouseMoved(const Ogre::Ray& ray);

    Ogre::SceneNode* mNode; //!< \brief The panel node
    Ogre::SceneNode* mPanelCameraNode; //!< \brief The node the camera should be attached to and look at mNode for this panel

protected:
    Gorilla::ScreenRenderable* mScreenRenderable;
    Ogre::Real mDistanceFromPanelToInteractWith; //!< \brief The distance the origin of a ray can be to interact with the panel.
};

}

#endif