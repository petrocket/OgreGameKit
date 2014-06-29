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

#ifndef Gui3DScreenPanel_H
#define Gui3DScreenPanel_H



#include <vector>

#include "Gorilla.h"

#include "Gui3D.h"
#include "Gui3DPanelContainer.h"

namespace Gui3D
{

//
class ScreenPanel : public PanelContainer
{
public:
    /** \brief Create a new layer on the @screen to display widgets.
    */
    ScreenPanel(Gui3D* gui, 
                Gorilla::Screen* screen, 
                const Ogre::Vector2& startPosition,
                const Ogre::Vector2& size,
                const Ogre::String& atlasName,
                const Ogre::String& name);

    ~ScreenPanel();

    /** \brief Give coordinates of the external mouse to this screen
    */
    bool injectMouseMoved(const Ogre::Real& x, const Ogre::Real& y);

    /** \brief Change the position of this screenPanel
    */
    void setPosition(const Ogre::Vector2& startPosition);

    virtual Button* makeButton(Ogre::Real x, 
                               Ogre::Real y, 
                               size_t width,
                               size_t height,
                               const Ogre::String& text);

    virtual Caption* makeCaption(Ogre::Real x, 
                                 Ogre::Real y,
                                 size_t width,
                                 size_t height,
                                 const Ogre::String& text,
                                 Gorilla::TextAlignment textAlign = Gorilla::TextAlign_Left,
                                 Gorilla::VerticalAlignment verticalAlign = Gorilla::VerticalAlign_Top);

    virtual Checkbox* makeCheckbox(Ogre::Real x,
                                   Ogre::Real y,
                                   size_t width,
                                   size_t height);

    virtual Combobox* makeCombobox(Ogre::Real x, 
                                   Ogre::Real y,
                                   size_t width,
                                   size_t height,
                                   const std::vector<Ogre::String>& items,
                                   unsigned int nbDisplayedElements);

    virtual InlineSelector* makeInlineSelector(Ogre::Real x,
                                               Ogre::Real y,
                                               size_t width,
                                               size_t height,
                                               const std::vector<Ogre::String>& items);

    virtual Listbox* makeListbox(Ogre::Real x, 
                                 Ogre::Real y,
                                 size_t width,
                                 size_t height,
                                 const std::vector<Ogre::String>& items,
                                 unsigned int nbDisplayedElements);

    virtual ProgressBar* makeProgressBar(Ogre::Real x, 
                                         Ogre::Real y,
                                         size_t width,
                                         size_t height);

    virtual ScrollBar* makeScrollBar(Ogre::Real x, 
                                     Ogre::Real y,
                                     size_t width,
                                     size_t height,
                                     Ogre::Real minValue,
                                     Ogre::Real maxValue);

    virtual TextZone* makeTextZone(Ogre::Real x,
                                   Ogre::Real y,
                                   size_t width,
                                   size_t height,
                                   const Ogre::String& text);

protected:
    Gorilla::Screen* mScreen;
    Ogre::Vector2 mStartPosition;
};

}

#endif