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


#include "Gui3DButton.h"

namespace Gui3D
{

using namespace std;

Button::Button(Ogre::Real x, 
               Ogre::Real y,
               size_t width,
               size_t height,
               const Ogre::String& text,
               Container* parentContainer)
               : PanelElementCallback(parentContainer), 
                 mIsActive(true), mIsClicked(false)
{
    mCaption = mParentContainer->getGUILayer()->createCaption(getColors()->buttonTextSize, x, y, text);
    mCaption->align(Gorilla::TextAlign_Centre);
    mCaption->vertical_align(Gorilla::VerticalAlign_Middle);
    mCaption->background(getColors()->transparent);
    mCaption->colour(getColors()->buttonText);

    mDesign = mParentContainer->getGUILayer()->createRectangle(x, y);

    setPosition(x, y);
    setSize(width, height);

    _actualize();
}


Button::~Button()
{
    mParentContainer->getGUILayer()->destroyCaption(mCaption);
    mParentContainer->getGUILayer()->destroyRectangle(mDesign);
}


void Button::injectKeyPressed(const OIS::KeyEvent& evt)
{
}
    

void Button::injectKeyReleased(const OIS::KeyEvent& evt)
{
}

void Button::injectKeys(vector<OIS::KeyCode>& keys)
{
}


void Button::injectMousePressed(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left)
    {
        if (!mIsActive)
            return;

        mIsClicked = true;

        _actualize();
    }
}


void Button::injectMouseReleased(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left)
    {
        mIsClicked = false;

        if (!mIsActive)
            return;

        setFocus(false);
        callCallback();
    }
}
    

bool Button::isOver(const Ogre::Vector2& pos)
{
    if (!mIsActive)
        return false;

    mOvered = mCaption->intersects(pos);
    _actualize();
    return mOvered;
}


void Button::setPosition(int left, int top)
{
    mCaption->left(left);
    mCaption->top(top);

    mDesign->left(left);
    mDesign->top(top);
}


void Button::setSize(int width, int height)
{
    mCaption->size(width, height);

    mDesign->width(width);
    mDesign->height(height);
}


void Button::setText(const Ogre::String& text)
{
    mCaption->text(text);
}
    

void Button::injectTimeAndMousePosition(double time, const Ogre::Vector2& pos)
{
}


void Button::highlight()
{
    mDesign->border_colour(getColors()->buttonBorderHighlight);
}


void Button::setActive(bool isActive)
{
    mIsActive = isActive;
    _actualize();
}


bool Button::getActive()
{
    return mIsActive;
}


Ogre::Vector2 Button::getPosition()
{
    return mDesign->position();
}


void Button::resetState()
{
    mIsClicked = false;
    mIsActive = true;

    PanelElement::resetState();
}


void Button::setBackgroundImage(const Ogre::String& overSpriteName, 
                                const Ogre::String& notOverSpriteName,
                                const Ogre::String& inactiveSpriteName,
                                const Ogre::String& clickedSpriteName)
{
    _clearRectangleDesign();
    
    if (overSpriteName.length() == 0     || overSpriteName == "none"     ||
        notOverSpriteName.length() == 0  || notOverSpriteName == "none"  ||
        inactiveSpriteName.length() == 0 || inactiveSpriteName == "none" ||
        clickedSpriteName.length() == 0  || clickedSpriteName == "none")
    {
        mOverSpriteName     = "";
        mNotOverSpriteName  = "";
        mInactiveSpriteName = "";
        mClickedSpriteName  = "";
    }
    else
    {
        mOverSpriteName     = overSpriteName;
        mNotOverSpriteName  = notOverSpriteName;
        mInactiveSpriteName = inactiveSpriteName;
        mClickedSpriteName  = clickedSpriteName;
        mDesign->border(0, getColors()->transparent);
    }

    _actualize();
}


void Button::_inactive()
{
    if (!mOverSpriteName.empty())
        mDesign->background_image(mInactiveSpriteName);
    else
    {
        mDesign->background_gradient(getColors()->buttonBackgroundInactiveGradientType, 
                                     getColors()->buttonBackgroundInactiveGradientStart,
                                     getColors()->buttonBackgroundInactiveGradientEnd);
        mDesign->border_colour(getColors()->buttonBorderInactive);
        mCaption->colour(getColors()->buttonTextInactive);
    }
}


void Button::_over()
{
    if (!mOverSpriteName.empty())
        mDesign->background_image(mOverSpriteName);
    else
    {
        mDesign->background_gradient(getColors()->buttonBackgroundOveredGradientType, 
                                     getColors()->buttonBackgroundOveredGradientStart,
                                     getColors()->buttonBackgroundOveredGradientEnd);
        mDesign->border(getColors()->buttonBorderSize, getColors()->buttonBorder);
        mCaption->colour(getColors()->buttonText);
    }
}


void Button::_unOver()
{
    if (!mOverSpriteName.empty())
        mDesign->background_image(mNotOverSpriteName);
    else
    {
        mDesign->background_gradient(getColors()->buttonBackgroundNotOveredGradientType, 
                                     getColors()->buttonBackgroundNotOveredGradientStart,
                                     getColors()->buttonBackgroundNotOveredGradientEnd);
        mDesign->border(getColors()->buttonBorderSize, getColors()->buttonBorder);
        mCaption->colour(getColors()->buttonText);
    }
}


void Button::_clicked()
{
    if (!mClickedSpriteName.empty())
        mDesign->background_image(mClickedSpriteName);
    else
    {
        mDesign->background_gradient(getColors()->buttonBackgroundClickedGradientType, 
                                     getColors()->buttonBackgroundClickedGradientStart,
                                     getColors()->buttonBackgroundClickedGradientEnd);
        mDesign->border(getColors()->buttonBorderSize, getColors()->buttonBorder);
        mCaption->colour(getColors()->buttonText);
    }
}


void Button::_actualize()
{
    if (mIsActive)
    {
        if (mIsClicked)
            _clicked();
        else
            mOvered ? _over() : _unOver();
    }
    else
        _inactive();
}


}