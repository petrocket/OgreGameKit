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

#ifndef Gui3DScrollBar_H
#define Gui3DScrollBar_H



#include <iostream>
#include <vector>

#include "Gorilla.h"
#include "Gui3DContainer.h"
#include "Gui3DPanelElementCallback.h"

#include "Gui3DMemberFunction.h"

namespace Gui3D
{

/*! class. ScrollBar
    desc.
        A Scroll Bar.
*/
class ScrollBar : public PanelElementCallback, public RectangleDesigned, public Captioned
{
public:
    ScrollBar(Ogre::Real x, 
              Ogre::Real y,
              size_t width,
              size_t height,
              Ogre::Real minValue,
              Ogre::Real maxValue,
              Container* parentContainer);

    ~ScrollBar();
    
    void injectKeyPressed(const OIS::KeyEvent& evt);
    
    void injectKeyReleased(const OIS::KeyEvent& evt);

    void injectKeys(std::vector<OIS::KeyCode>& keys);
    
    void injectMousePressed(const OIS::MouseEvent& evt, 
                            OIS::MouseButtonID id);
    
    void injectMouseReleased(const OIS::MouseEvent& evt, 
                             OIS::MouseButtonID id);
    
    bool isOver(const Ogre::Vector2& pos);
    
    void injectTimeAndMousePosition(double time, const Ogre::Vector2& pos);

    double getValue();

    Ogre::Vector2 getPosition();
    
    void setSize(int width, int height);

    void setPosition(int left, int top);

    void setValue(Ogre::Real value);

    void setMinMaxValue(Ogre::Real min, Ogre::Real max);

    /** \brief Set the step of the values
    */
    void setStep(Ogre::Real step);

    /** \brief Configure the displayed value precision.
    With @fixedValuePrecision true, the displayed will be
        str << fixed << setprecision(precision) << value;
    else, it will be
        str << setprecision(precision) << value;
    */
    void setDisplayedPrecision(int precision, bool fixedValuePrecision);

    /** \brief Set if the value should be displayed on the widget
    */
    void setDisplayValue(bool displayValue);

    /** \brief Set the cursor width and height depending on the element width and height.
    */
    void setCursorWidthHeightPercent(Ogre::Real widthPercent, 
                                     Ogre::Real heightPercent);

    /** \brief Define the callback when a release click has been performed
    */
    template<typename T>
    void setValueChangedCallback(T* object, bool(T::*method)(PanelElement*))
    {
        changeCallback(new MemberFunction<T>(object, method));
    }

    void highlight();

    /** \brief Set the backgrounds images of the cursor from the texture atlas. 
        note.
            To remove the images pass on "none" or a empty string to at least one parameter.
            Panel Colors will then be used.
    */
    void setCursorSprites(const Ogre::String& cursorOveredSpriteName,
                          const Ogre::String& cursorNotOveredSpriteName,
                          const Ogre::String& cursorSelectedOveredSpriteName);

    /** \brief Set if the callback function should be called even when the
    user is selecting the value (not only when he released the mouse)
    */
    void setCallCallbackOnSelectingValue(bool callCallbackOnSelectingalue);

protected:
    Ogre::Real mMinValue;

    Ogre::Real mMaxValue;

    Ogre::Real mValue;

    Ogre::Real mCursorWidthPercent; //! \brief The % of the element width the cursor width should be

    Ogre::Real mCursorHeightPercent; //! \brief The % of the element height the cursor height should be

    Ogre::Real mStep; //!< \brief The value will always be a multiple of the step

    int mValuePrecision; //!< \brief Set the precision of the value inside the scrollbar

    bool mFixedValuePrecision; //!< \brief Fixed point notation for the value inside the scrollbar

    bool mDisplayValue; //!< \brief Display the value of the scrollbar

    bool mCallCallbackOnSelectingValue; //!< \brief Call the callback when selecting the value

    Gorilla::Rectangle* mCursor; //!< \brief The movable cursor of the scrollbar

    Gorilla::Rectangle* mBar; //!< \brief The bar behind the cursor

    Ogre::String mCursorOveredSpriteName;

    Ogre::String mCursorNotOveredSpriteName;

    Ogre::String mCursorSelectedSpriteName;

    void _unOverCursor();

    void _overCursor();

    void _selectCursor();

    void _clearCursorDesign(); //!< \brief Clear the cursor design for a proper change of background image / colors.

    void _actualize();

    void _actualizeBar(); //!< \brief Actualize the progressBar behind the cursor

    void _actualizeValue(); //!< \brief Actualize the value by taking care of the step, min and max value

    void _actualizeDisplayedValue(); //!< \brief Actualize the displayed value
};

}

#endif
