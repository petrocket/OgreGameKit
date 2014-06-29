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

#ifndef Gui3DListbox_H
#define Gui3DListbox_H

#include "Gui3DVerticalSelector.h"

#include <map>

namespace Gui3D
{

/** class. Listbox
    desc.
        A Listbox which allows the user to make multiple choices.
*/
class Listbox : public VerticalSelector
{
public:
    /** \brief Use Panel::makeListbox
    */
    Listbox(Ogre::Real x, 
            Ogre::Real y,
            size_t width,
            size_t height,
            const std::vector<Ogre::String>& items,
            unsigned int nbDisplayedElements,
            Container* parentContainer);
    
    /** \brief Use Panel::destroyListbox
    */
    ~Listbox();

    void injectKeyPressed(const OIS::KeyEvent& evt);

    void injectKeyReleased(const OIS::KeyEvent& evt);

    void injectKeys(std::vector<OIS::KeyCode>& keys);

    void injectMousePressed(const OIS::MouseEvent& evt, 
                            OIS::MouseButtonID id);

    bool isOver(const Ogre::Vector2& pos);

    void setCurrentValue(Ogre::String& itemName);

    void setCurrentValues(std::vector<Ogre::String>& itemsName);

    std::vector<Ogre::String> getValues();

    void setFocus(bool hasFocus);

    void highlight();

protected:
    bool addedMode; //!< \brief The Listbox will add elements to previous selected elements

    std::map<int, bool> mSelectedElements;

    void _actualize();
};

}

#endif