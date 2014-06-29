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


#include "Gui3DCheckbox.h"

namespace Gui3D
{

using namespace std;

Checkbox::Checkbox(Ogre::Real x, 
                   Ogre::Real y,
                   size_t width,
                   size_t height,
                   Container* parentContainer)
                   : PanelElementCallback(parentContainer), mChecked(false)
{
    mDesign = mParentContainer->getGUILayer()->createRectangle(x, y, width, height);

    setPosition(x, y);
    setSize(width, height);
}


Checkbox::~Checkbox()
{
    mParentContainer->getGUILayer()->destroyRectangle(mDesign);
}


void Checkbox::injectKeyPressed(const OIS::KeyEvent& evt)
{
}


void Checkbox::injectKeyReleased(const OIS::KeyEvent& evt)
{
}


void Checkbox::injectKeys(vector<OIS::KeyCode>& keys)
{
}


void Checkbox::injectMousePressed(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left)
    {
        setChecked(!mChecked);
    }
}


void Checkbox::injectMouseReleased(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
}


bool Checkbox::isOver(const Ogre::Vector2& pos)
{
    mOvered = mDesign->intersects(pos);
    _actualize();
    return mOvered;
}


bool Checkbox::getChecked()
{
    return mChecked;
}


Ogre::Vector2 Checkbox::getPosition()
{
    return mDesign->position();
}


void Checkbox::setChecked(bool checked)
{
    mChecked = checked;
    _actualize();
    callCallback();
}


void Checkbox::setSize(int width, int height)
{
    mDesign->width(width);
    mDesign->height(height);
}


void Checkbox::setPosition(int left, int top)
{
    mDesign->left(left);
    mDesign->top(top);
}


void Checkbox::injectTimeAndMousePosition(double time, const Ogre::Vector2& pos)
{
}


void Checkbox::highlight()
{
    mDesign->border_colour(getColors()->checkboxBorderHighlight);
}


void Checkbox::_actualize()
{
    if (!mChecked)
        mOvered ? _over() : _unOver();
    else
        mOvered ? _checkOver() : _checkUnOver();
}

}