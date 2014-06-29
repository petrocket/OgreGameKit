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

#ifndef Gui3DVerticalSelector_H
#define Gui3DVerticalSelector_H



#include <vector>

#include "Gorilla.h"
#include "Gui3DMultipleElementsSelector.h"

#include "Gui3DCaption.h"

namespace Gui3D
{

/*! class. VerticalSelector
    desc.
        An abstract class for all vertical selector such as Listbox or Combobox.
*/
class VerticalSelector : public MultipleElementsSelector
{
public:
    VerticalSelector(Ogre::Real x, 
                     Ogre::Real y,
                     size_t width,
                     size_t height,
                     const std::vector<Ogre::String>& items,
                     unsigned int nbDisplayedElements,
                     Container* parentContainer);
    
    ~VerticalSelector();

    Ogre::Vector2 getPosition();

    void setPosition(int left, int top);
    
    void setSize(int width, int height);
    
protected:
    std::vector<Caption*> mCaptions; //!< \brief Contains the displayed caption of the combobox

    unsigned int mNumTopVisibleElement; //!< \brief Number of the top element that is shown

    unsigned int mMaxNumberOfVisibleElements; //!< \brief Number of elements that can be seen in the selector

    /** \brief Callback for allElementsGoTop method 
    */
    bool allElementsGoTopCallback(PanelElement*);

    /** \brief Callback for allElementsGoDown method
    */
    bool allElementsGoDownCallback(PanelElement*);

    /** \brief Visibles elements go top, then the down hidding elements
    are started to be seen
    */
    void allElementsGoTop();

    /** \brief Visibles elements go down, then the top hidding elements
    are started to be seen
    */
    void allElementsGoDown();
};

}

#endif