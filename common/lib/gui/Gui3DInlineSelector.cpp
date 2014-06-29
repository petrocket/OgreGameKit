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


#include "Gui3DInlineSelector.h"

namespace Gui3D
{

static float INLINE_SELECTOR_BUTTON_CAPTION_SPACING = 0.02;
static float INLINE_SELECTOR_CAPTION_X_SIZE         = 0.8;
static float INLINE_SELECTOR_BUTTON_Y_SIZE          = 0.8;

using namespace std;

InlineSelector::InlineSelector(Ogre::Real x, 
                               Ogre::Real y,
                               size_t width,
                               size_t height,
                               const vector<Ogre::String>& items,
                               Container* parentContainer)
                               : MultipleElementsSelector(x, y, width, height, items, parentContainer), 
                                  mIndiceSelectedElement(-1)
{
    mCaption = mParentContainer->getGUILayer()->createCaption(getColors()->inlineselectorTextSize, 0, 0, "");
    mCaption->align(Gorilla::TextAlign_Centre);
    mCaption->vertical_align(Gorilla::VerticalAlign_Middle);
    mCaption->background(getColors()->transparent);
    mCaption->colour(getColors()->inlineselectorText);

    mDesign->background_gradient(getColors()->inlineselectorBackgroundGradientType,
                                 getColors()->inlineselectorBackgroundGradientStart,
                                 getColors()->inlineselectorBackgroundGradientEnd);
    mDesign->border(getColors()->inlineselectorBorderSize,
                    getColors()->inlineselectorBorder);

    mPreviousElementsButton->text("<");
    mPreviousElementsButton->setPressedCallback(this, &InlineSelector::goPreviousElementCallback);

    mNextElementsButton->text(">");
    mNextElementsButton->setPressedCallback(this, &InlineSelector::goNextElementCallback);

    if (items.size() > 0)
        mIndiceSelectedElement = 0;

    _actualize();

    setPosition(x, y);
    setSize(width, height);
}


void InlineSelector::setSize(int width, int height)
{
    mDesign->width(width);
    mDesign->height(height);

    int x = mDesign->left();
    int y = mDesign->top();

    int sizeXButtons = width * (1 - INLINE_SELECTOR_CAPTION_X_SIZE 
                                - 2 * INLINE_SELECTOR_BUTTON_CAPTION_SPACING) / 2;
    int sizeYButtons                    =     height * INLINE_SELECTOR_BUTTON_Y_SIZE;
    int decalXButtonPreviousElements    = x;
    int decalYButtonPreviousElements    = y + height * ((1 - INLINE_SELECTOR_BUTTON_Y_SIZE) / 2);
    int decalXButtonNextElements        = x + width - sizeXButtons;
    int decalYButtonNextElements        = decalYButtonPreviousElements;

    int sizeXCaption =      width * INLINE_SELECTOR_CAPTION_X_SIZE;
    int sizeYCaption =      height;
    int decalXCaption = x + width - (sizeXCaption + sizeXButtons + INLINE_SELECTOR_BUTTON_CAPTION_SPACING * width);
    int decalYCaption = y;

    mCaption->left(decalXCaption);
    mCaption->top(decalYCaption);
    mCaption->size(sizeXCaption, sizeYCaption);

    mPreviousElementsButton->setPosition(decalXButtonPreviousElements, decalYButtonPreviousElements);
    mPreviousElementsButton->setSize(sizeXButtons, sizeYButtons);

    mNextElementsButton->setPosition(decalXButtonNextElements, decalYButtonNextElements);
    mNextElementsButton->setSize(sizeXButtons, sizeYButtons);
}


InlineSelector::~InlineSelector()
{
    mParentContainer->getGUILayer()->destroyCaption(mCaption);
}


void InlineSelector::goPreviousElement()
{
    if (mIndiceSelectedElement > 0)
    {
        mIndiceSelectedElement--;
        _actualize();
        callCallback();
    }
}


void InlineSelector::goNextElement()
{
    if (mIndiceSelectedElement < mValues.size()-1)
    {
        mIndiceSelectedElement++;
        _actualize();
        callCallback();
    }
}


bool InlineSelector::goPreviousElementCallback(PanelElement* e)
{
    goPreviousElement();
    return true;
}


bool InlineSelector::goNextElementCallback(PanelElement* e)
{
    goNextElement();
    return true;
}


void InlineSelector::injectKeyPressed(const OIS::KeyEvent& evt)
{
}


void InlineSelector::injectKeyReleased(const OIS::KeyEvent& evt)
{
}


void InlineSelector::injectKeys(vector<OIS::KeyCode>& keys)
{
}


bool InlineSelector::isOver(const Ogre::Vector2& pos)
{
    if (MultipleElementsSelector::isOver(pos))
        return true;

    mOvered = mDesign->intersects(pos);
    if (mOvered)
        mActualOveredElement = INLINE_SELECTOR_CAPTION;
    else
        mActualOveredElement = MULTIPLE_ELEMENT_SELECTOR_NO_ELEMENT;
    return mOvered;
}


void InlineSelector::setPosition(int left, int top)
{
    Ogre::Real decalLeft = left - mDesign->left();
    Ogre::Real decalTop  = top - mDesign->top();

    mCaption->left(mCaption->left()+decalLeft);
    mCaption->top(mCaption->top()+decalTop);

    MultipleElementsSelector::setPosition(left, top);
}


void InlineSelector::setCurrentValue(Ogre::String itemName)
{
    for (size_t i=0; i < mValues.size(); i++)
    {
        if (mValues[i] == itemName)
        {
            mIndiceSelectedElement = i;
            _actualize();
            break;
        }
    }
}


Ogre::Vector2 InlineSelector::getPosition()
{
    return mPreviousElementsButton->getPosition();
}


Ogre::String InlineSelector::getValue()
{
    if (mIndiceSelectedElement == INLINE_SELECTOR_NO_SELECTED_ELEMENT)
        return "";
    return mValues[mIndiceSelectedElement];
}


void InlineSelector::highlight()
{
    mDesign->border_colour(getColors()->inlineselectorBorderHighlight);
}


void InlineSelector::_actualize()
{
    if (mIndiceSelectedElement == INLINE_SELECTOR_NO_SELECTED_ELEMENT)
        return;

    mCaption->text(mValues[mIndiceSelectedElement]);
}

}