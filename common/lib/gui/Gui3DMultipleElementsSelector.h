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

#ifndef Gui3DMultipleElementsSelector_H
#define Gui3DMultipleElementsSelector_H

#include "Gui3DPanelElementCallback.h"
#include "Gui3DButton.h"

namespace Gui3D
{

/*! class. MultipleElementsSelector
    desc.
        An abstract class for a selector with multiple elements and buttons.
*/
class MultipleElementsSelector : public PanelElementCallback, public RectangleDesigned
{
public:
    MultipleElementsSelector(Ogre::Real x, 
                             Ogre::Real y,
                             size_t width,
                             size_t height,
                             const std::vector<Ogre::String>& items,
                             Container* parentContainer);

    ~MultipleElementsSelector();

    virtual void injectMousePressed(const OIS::MouseEvent& evt, 
                                    OIS::MouseButtonID id);

    virtual void injectMouseReleased(const OIS::MouseEvent& evt, 
                                     OIS::MouseButtonID id);

    virtual bool isOver(const Ogre::Vector2& pos);

    /** \brief Add an item to the container
    */
    virtual void addItem(Ogre::String itemName);

    void injectTimeAndMousePosition(double time, const Ogre::Vector2& pos);

    void resetState();

    /** \brief Set the backgrounds images of the previous and next buttons from the texture atlas. 
        note.
            To remove the images pass "none" or a empty string to at least one parameter
    */
    void setBackgroundImageButtons(const Ogre::String& previousButtonOveredSpriteName,
                                   const Ogre::String& previousButtonNotOveredSpriteName,
                                   const Ogre::String& previousButtonInactiveSpriteName,
                                   const Ogre::String& previousButtonClickedSpriteName,
                                   const Ogre::String& nextButtonOveredSpriteName,
                                   const Ogre::String& nextButtonNotOveredSpriteName,
                                   const Ogre::String& nextButtonInactiveSpriteName,
                                   const Ogre::String& nextButtonClickedSpriteName);

    virtual void setPosition(int left, int top);

    /** \brief Define the callback when a value has changed
    */
    template<typename T>
    void setValueChangedCallback(T* object, bool(T::*method)(PanelElement*))
    {
        changeCallback(new MemberFunction<T>(object, method));
    }

    /** \brief Be careful when using this. Undesired behaviour
    can happened. You should then just use it to set some
    alignment/colors options.
    Return the previous elements button.
    */
    Button* getPreviousElementsButton();

    /** \brief Be careful when using this. Undesired behaviour
    can happened. You should then just use it to set some
    alignment/colors options.
    Return the next elements button.
    */
    Button* getNextElementsButton();

protected:
    enum
    {
        MULTIPLE_ELEMENT_SELECTOR_NEXT_ELEMENT_BUTTON = -3,
        MULTIPLE_ELEMENT_SELECTOR_PREVIOUS_ELEMENT_BUTTON,
        MULTIPLE_ELEMENT_SELECTOR_NO_ELEMENT
    };

    std::vector<Ogre::String> mValues; //!< \brief Contains all the string elements that are in the combobox

    /** \brief The actual overed element.
    If it's >= 0, it's a selector value element, else it's a speial case.
    See implementation : can be NO_ELEMENT, PREVIOUS_ELEMENT_BUTTON or NEXT_ELEMENT_BUTTON
    */
    int mActualOveredElement;

    Button* mPreviousElementsButton;

    Button* mNextElementsButton;
};

}

#endif