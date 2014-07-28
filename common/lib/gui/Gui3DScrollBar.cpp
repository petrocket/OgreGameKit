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


#include "Gui3DScrollBar.h"

#include "Gui3DUtils.h"

namespace Gui3D
{

using namespace std;

ScrollBar::ScrollBar(Ogre::Real x, 
                     Ogre::Real y,
                     size_t width,
                     size_t height,
                     Ogre::Real minValue,
                     Ogre::Real maxValue,
                     Container* parentContainer)
                     : PanelElementCallback(parentContainer), 
                       mMinValue(minValue), mMaxValue(maxValue),
                       mCursorWidthPercent(0.05), mCursorHeightPercent(1.5),
                       mValue(minValue), mCursor(NULL), mStep(0.), 
                       mValuePrecision(0), mFixedValuePrecision(false),
                       mCallCallbackOnSelectingValue(false), mDisplayValue(true)
{
    mCaption = mParentContainer->getGUILayer()->createCaption(getColors()->scrollbarTextSize, x, y, "");
    mCaption->align(Gorilla::TextAlign_Centre);
    mCaption->vertical_align(Gorilla::VerticalAlign_Middle);
    mCaption->background(getColors()->transparent);
    mCaption->colour(getColors()->scrollbarText);

    mDesign = mParentContainer->getGUILayer()->createRectangle(x, y);
    mDesign->background_gradient(getColors()->scrollbarBackgroundGradientType,
                                 getColors()->scrollbarBackgroundGradientStart,
                                 getColors()->scrollbarBackgroundGradientEnd);
    mDesign->border(getColors()->scrollbarBorderSize, 
                    getColors()->scrollbarBorder);

    mBar = mParentContainer->getGUILayer()->createRectangle(x, y);
    mBar->background_gradient(getColors()->scrollbarProgressbarGradientType,
                              getColors()->scrollbarProgressbarGradientStart,
                              getColors()->scrollbarProgressbarGradientEnd);

    mCursor = mParentContainer->getGUILayer()->createRectangle(x, y);

    setPosition(x, y);
    setSize(width, height);

    _actualizeDisplayedValue();
    _actualize();
}


ScrollBar::~ScrollBar()
{
    mParentContainer->getGUILayer()->destroyCaption(mCaption);
    mParentContainer->getGUILayer()->destroyRectangle(mDesign);

    mParentContainer->getGUILayer()->destroyRectangle(mBar);
    mParentContainer->getGUILayer()->destroyRectangle(mCursor);
}


void ScrollBar::injectKeyPressed(const OIS::KeyEvent& evt)
{
}


void ScrollBar::injectKeyReleased(const OIS::KeyEvent& evt)
{
}


void ScrollBar::injectKeys(vector<OIS::KeyCode>& keys)
{
}


void ScrollBar::injectMousePressed(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
}


void ScrollBar::injectMouseReleased(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
    setFocus(false);

    callCallback();
}

void ScrollBar::injectTouchPressed(const OIS::MultiTouchEvent& evt)
{
    setFocus(false);
    
    callCallback();
}

bool ScrollBar::isOver(const Ogre::Vector2& pos)
{
    mOvered = mCursor->intersects(pos) || mCaption->intersects(pos);
    _actualize();
    return mOvered;
}


void ScrollBar::injectTimeAndMousePosition(double time, const Ogre::Vector2& pos)
{
    // Only move when has focus
    if (mHasFocus)
    {
        // Consider the cursor in the container
        Ogre::Real cursorPositionX = pos.x;
        // Cursor out on the left
        if (pos.x < mCaption->left())
            cursorPositionX = mCaption->left();
        // Cursor out on the right
        else if (pos.x > mCaption->left() + mCaption->width())
            cursorPositionX = mCaption->left() + mCaption->width();
        // The middle of the cursor has to be on mouse position
        mCursor->position(cursorPositionX - (mCursor->width() / 2), 
            mCursor->position().y);

        // Actualize the value
        Ogre::Real posBeginScrollBarX = mCaption->left();
        Ogre::Real posEndScrollBarX = mCaption->left() + mCaption->width();
    
        // Calculate the normalized position of the cursor in its container
        Ogre::Real cursorNormalizedPosition = (cursorPositionX - posBeginScrollBarX) /
            (posEndScrollBarX - posBeginScrollBarX);

        mValue = mMinValue + (mMaxValue-mMinValue) * cursorNormalizedPosition;

        // Take care of the step ; Return the closer min step multiple value
        if (mStep != 0)
        {
            mValue = mMinValue + round((mValue-mMinValue) / mStep) * mStep;
            // Make sure the value is not superior to mMaxValue
            if (mValue > mMaxValue)
                mValue = mMaxValue;
        }

        if (mCallCallbackOnSelectingValue)
            callCallback();

        _actualizeDisplayedValue();
        _actualizeBar();
    }
}


void ScrollBar::setPosition(int left, int top)
{
    Ogre::Real decalLeft = left - mCaption->left();
    Ogre::Real decalTop = top - mCaption->top();

    mCaption->left(left);
    mCaption->top(top);

    mDesign->left(left);
    mDesign->top(top);

    mBar->left(left);
    mBar->top(top);

    Ogre::Real newCursorLeft = mCursor->left() + decalLeft;
    Ogre::Real newCursorTop = mCursor->top() + decalTop;

    mCursor->left(newCursorLeft);
    mCursor->top(newCursorTop);
}


double ScrollBar::getValue()
{
    return mValue;
}


Ogre::Vector2 ScrollBar::getPosition()
{
    return mDesign->position();
}


void ScrollBar::setValue(Ogre::Real value)
{
    Ogre::Real newCursorPositionX = 0;

    if (value < mMinValue)
    {
        mValue = mMinValue;
        newCursorPositionX = mCaption->left();
    }
    else if (value > mMaxValue)
    {
        mValue = mMaxValue;
        newCursorPositionX = mCaption->left() + mCaption->width();
    }
    else
    {
        mValue = value;

        // Normalize value
        Ogre::Real normalizedValue = (mValue - mMinValue) / (mMaxValue - mMinValue);
        newCursorPositionX = mCaption->left() + normalizedValue * mCaption->width();
    }

    mCursor->position(newCursorPositionX, mCursor->position().y);

    _actualizeValue();
    _actualizeDisplayedValue();
    _actualizeBar();

    callCallback();
}


void ScrollBar::setMinMaxValue(Ogre::Real min, 
    Ogre::Real max)
{
    mMinValue = min;
    mMaxValue = max;

    _actualizeValue();
    _actualizeDisplayedValue();
}


void ScrollBar::setDisplayedPrecision(int precision, bool fixedValuePrecision)
{
    mValuePrecision = precision;
    mFixedValuePrecision = fixedValuePrecision;

    _actualizeValue();
    _actualizeDisplayedValue();
}


void ScrollBar::setDisplayValue(bool displayValue)
{
    mDisplayValue = displayValue;
    
    if (!mDisplayValue)
        mCaption->text("");
    else
        _actualizeDisplayedValue();
}


void ScrollBar::setSize(int width, int height)
{
    mCaption->size(width, height);
    
    mDesign->width(width);
    mDesign->height(height);

    Ogre::Real decalCursorY = (mCursorHeightPercent - 1) / 2;
    
    mCursor->width(width * mCursorWidthPercent);
    mCursor->top(mDesign->top() - height * decalCursorY);
    mCursor->height(height * mCursorHeightPercent);

    mBar->height(height);

    _actualizeBar();
}


void ScrollBar::setStep(Ogre::Real step)
{
    mStep = step;

    _actualizeValue();
    _actualizeDisplayedValue();
    callCallback();
}


void ScrollBar::setCallCallbackOnSelectingValue(bool callCallbackOnSelectingValue)
{
    mCallCallbackOnSelectingValue = callCallbackOnSelectingValue;
}


void ScrollBar::highlight()
{
    mDesign->border_colour(getColors()->scrollbarBorderHighlight);
}


void ScrollBar::setCursorSprites(const Ogre::String& cursorOveredSpriteName,
    const Ogre::String& cursorNotOveredSpriteName,
    const Ogre::String& cursorSelectedSpriteName)
{
    _clearCursorDesign();

    if (cursorOveredSpriteName.length() == 0    || cursorOveredSpriteName == "none"     ||
        cursorNotOveredSpriteName.length() == 0 || cursorNotOveredSpriteName == "none"  ||
        cursorSelectedSpriteName.length() == 0  || cursorSelectedSpriteName == "none")
    {
        mCursorOveredSpriteName     = "";
        mCursorNotOveredSpriteName  = "";
        mCursorSelectedSpriteName   = "";
    }
    else
    {
        mCursorOveredSpriteName     = cursorOveredSpriteName;
        mCursorNotOveredSpriteName  = cursorNotOveredSpriteName;
        mCursorSelectedSpriteName   = cursorSelectedSpriteName;
    }

    _actualize();
}


void ScrollBar::_actualize()
{
    if (mHasFocus)
        _selectCursor();
    else
        mOvered ? _overCursor() : _unOverCursor();
}


void ScrollBar::_actualizeBar()
{
    Ogre::Real barWidth = mCursor->left() - mCaption->left();
    // The middle of the cursor is at the mouse position. The left
    //  of the cursor can then be outside of the caption.
    if (barWidth < 0)
        barWidth = 0;
    mBar->width(barWidth);
}


void ScrollBar::_actualizeValue()
{
    // Take care of the step ; Return the closer min step multiple value
    if (mStep != 0)
    {
        mValue = mMinValue + round((mValue-mMinValue) / mStep) * mStep;
        // Make sure the value is not superior to mMaxValue
        if (mValue > mMaxValue)
            mValue = mMaxValue;
    }
}


void ScrollBar::_actualizeDisplayedValue()
{
    if (!mDisplayValue)
        return;

    ostringstream s;
    if (mFixedValuePrecision)
        s << fixed;
    
    s << setprecision(mValuePrecision);
    s << getValue();
    mCaption->text(s.str());
}


void ScrollBar::setCursorWidthHeightPercent(Ogre::Real widthPercent, 
    Ogre::Real heightPercent)
{
    mCursorWidthPercent = widthPercent;
    mCursorHeightPercent = heightPercent;

    // TODO : Make a _actualizeCursorWidth function instead of "resizing" with same size to actualize cursor position ?
    setSize(mDesign->width(), mDesign->height());
}


void ScrollBar::_clearCursorDesign()
{
    mCursor->background_colour(Ogre::ColourValue::White);
}


void ScrollBar::_unOverCursor()
{
    if (mCursorNotOveredSpriteName == "none" || mCursorNotOveredSpriteName.length() == 0)
    {
        mCursor->background_gradient(getColors()->scrollbarCursorNotOveredGradientType,
                                     getColors()->scrollbarCursorNotOveredGradientStart,
                                     getColors()->scrollbarCursorNotOveredGradientEnd);
        mCursor->border(getColors()->scrollbarCursorBorderSize,
                        getColors()->scrollbarCursorBorder);
    }
    else
        mCursor->background_image(mCursorNotOveredSpriteName);
}


void ScrollBar::_overCursor()
{
    if (mCursorOveredSpriteName == "none" || mCursorOveredSpriteName.length() == 0)
    {
        mCursor->background_gradient(getColors()->scrollbarCursorOveredGradientType,
                                     getColors()->scrollbarCursorOveredGradientStart,
                                     getColors()->scrollbarCursorOveredGradientEnd);
        mCursor->border(getColors()->scrollbarCursorBorderSize,
                        getColors()->scrollbarCursorBorder);
    }
    else
        mCursor->background_image(mCursorOveredSpriteName);
}


void ScrollBar::_selectCursor()
{
    if (mCursorSelectedSpriteName == "none" || mCursorSelectedSpriteName.length() == 0)
    {
        mCursor->background_gradient(getColors()->scrollbarCursorSelectedGradientType,
                                     getColors()->scrollbarCursorSelectedGradientStart,
                                     getColors()->scrollbarCursorSelectedGradientEnd);
        mCursor->border(getColors()->scrollbarCursorBorderSize,
                        getColors()->scrollbarCursorBorder);
    }
    else
        mCursor->background_image(mCursorSelectedSpriteName);
}

}