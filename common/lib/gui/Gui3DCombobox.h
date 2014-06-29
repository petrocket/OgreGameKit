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

#ifndef Gui3DCombobox_H
#define Gui3DCombobox_H



#include <vector>

#include "Gorilla.h"
#include "Gui3DContainer.h"
#include "Gui3DVerticalSelector.h"

#include "Gui3DButton.h"
#include "Gui3DCaption.h"

#include "Gui3DMemberFunction.h"

namespace Gui3D
{

/*! class. Combobox
    desc.
        A Combobox which allows the user to make one choice.
*/
class Combobox : public VerticalSelector
{
public:
    /** \brief Use Panel::makeCombobox
    */
    Combobox(Ogre::Real x, 
             Ogre::Real y,
             size_t width,
             size_t height,
             const std::vector<Ogre::String>& items,
             unsigned int nbDisplayedElements,
             Container* parentContainer);
    
    /** \brief Use Panel::deleteCombobox
    */
    ~Combobox();

    void injectKeyPressed(const OIS::KeyEvent& evt);

    void injectKeyReleased(const OIS::KeyEvent& evt);

    void injectKeys(std::vector<OIS::KeyCode>& keys);

    void injectMousePressed(const OIS::MouseEvent& evt, 
                            OIS::MouseButtonID id);
    
    bool isOver(const Ogre::Vector2& pos);

    /** \brief Return the value of the combobox
    */
    Ogre::String getValue();

    /** \brief Set the current value of this combobox. If the element 
    isn't found, nothing is changed.
    */
    void setCurrentValue(Ogre::String& itemName);

    void highlight();
    
protected:
    unsigned int mActualSelectedElement; //!< \brief The actual selected element

    void _actualize();
};

}

#endif