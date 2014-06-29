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


#include "Gui3DCheckboxText.h"

namespace Gui3D
{

CheckboxText::CheckboxText(Ogre::Real x, 
                           Ogre::Real y,
                           size_t width,
                           size_t height, 
                           char checkedSymbol,
                           Container* parentContainer)
                           : Checkbox(x, y, width, height, parentContainer), 
                             mCheckedSymbol(checkedSymbol)
{
    mCaption = mParentContainer->getGUILayer()->createCaption(getColors()->checkboxTextSize, x, y, "");
    mCaption->size(width, height);
    mCaption->align(Gorilla::TextAlign_Centre);
    mCaption->vertical_align(Gorilla::VerticalAlign_Middle);
    mCaption->background(getColors()->transparent);
    mCaption->colour(getColors()->checkboxText);

    _actualize();
}


CheckboxText::~CheckboxText()
{
    mParentContainer->getGUILayer()->destroyCaption(mCaption);
}


void CheckboxText::setSize(int width, int height)
{
    Checkbox::setSize(width, height);
    
    mCaption->size(width, height);
}


void CheckboxText::setPosition(int left, int top)
{
    Checkbox::setPosition(left, top);

    mCaption->left(left);
    mCaption->top(top);
}


void CheckboxText::_checkUnOver()
{
    _unOver();
    Ogre::String s;
    s += mCheckedSymbol;
    mCaption->text(s);
}


void CheckboxText::_checkOver()
{
    _over();
    Ogre::String s;
    s += mCheckedSymbol;
    mCaption->text(s);
}


void CheckboxText::_unOver()
{
    mCaption->text("");

    if (mChecked)
    {
        mDesign->background_gradient(getColors()->checkboxSelectedNotOveredGradientType,
                                     getColors()->checkboxSelectedNotOveredGradientStart,
                                     getColors()->checkboxSelectedNotOveredGradientEnd);
    }
    else
    {
        mDesign->background_gradient(getColors()->checkboxNotOveredGradientType,
                                     getColors()->checkboxNotOveredGradientStart,
                                     getColors()->checkboxNotOveredGradientEnd);
    }

    mDesign->border(getColors()->checkboxBorderSize, getColors()->checkboxBorder);
}


void CheckboxText::_over()
{
    mCaption->text("");

    if (mChecked)
    {
        mDesign->background_gradient(getColors()->checkboxSelectedOveredGradientType,
                                     getColors()->checkboxSelectedOveredGradientStart,
                                     getColors()->checkboxSelectedOveredGradientEnd);
    }
    else
    {
        mDesign->background_gradient(getColors()->checkboxOveredGradientType,
                                     getColors()->checkboxOveredGradientStart,
                                     getColors()->checkboxOveredGradientEnd);
    }
    mDesign->border(getColors()->checkboxBorderSize,
                    getColors()->checkboxBorder);
}

}