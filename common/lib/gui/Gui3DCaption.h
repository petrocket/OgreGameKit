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

#ifndef Gui3DCaption_H
#define Gui3DCaption_H



#include <vector>

#include "Gorilla.h"
#include "Gui3DContainer.h"
#include "Gui3DPanelElement.h"

namespace Gui3D
{

/*! class. Caption
    desc.
        A simple Caption
*/
class Caption : public PanelElement, public RectangleDesigned, public Captioned 
{
public:
    Caption(Ogre::Real x, 
            Ogre::Real y,
            size_t width,
            size_t height,
            const Ogre::String& text, 
            Container* parentContainer,
            Gorilla::TextAlignment textAlign = Gorilla::TextAlign_Left,
            Gorilla::VerticalAlignment verticalAlign = Gorilla::VerticalAlign_Top);

    ~Caption();

    void injectKeyPressed(const OIS::KeyEvent& evt);
    
    void injectKeyReleased(const OIS::KeyEvent& evt);

    void injectKeys(std::vector<OIS::KeyCode>& keys);

    void injectMousePressed(const OIS::MouseEvent& evt, 
                            OIS::MouseButtonID id);

    void injectMouseReleased(const OIS::MouseEvent& evt, 
                             OIS::MouseButtonID id);

    bool isOver(const Ogre::Vector2& pos);

    void setSize(int width, int height);
    
    void setPosition(int left, int top);

    Ogre::Vector2 getPosition();

    void injectTimeAndMousePosition(double time, const Ogre::Vector2& pos);

    void highlight();

    void text(Ogre::String s);

    Ogre::String text();

    /** \brief Set the background to a sprite from the texture atlas.
        note.
            To remove the images pass on "none" or a empty string to the sprite name.
    */
    void setBackgroundImage(const Ogre::String& backgroundSpriteName);

protected:
    Ogre::String mBackgroundSpriteName;

    void _actualize();
};

}

#endif