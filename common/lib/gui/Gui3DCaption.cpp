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


#include "Gui3DCaption.h"

namespace Gui3D
{

using namespace std;

Caption::Caption(Ogre::Real x, 
                 Ogre::Real y,
                 size_t width,
                 size_t height,
                 const Ogre::String& text,
                 Container* parentContainer,
                 Gorilla::TextAlignment textAlign,
                 Gorilla::VerticalAlignment verticalAlign)
                 : PanelElement(parentContainer)
{
    mCaption = mParentContainer->getGUILayer()->createCaption(getColors()->captionTextSize, x, y, text);
    mCaption->size(width, height);
    mCaption->align(textAlign);
    mCaption->vertical_align(verticalAlign);
    mCaption->background(getColors()->transparent);
    mCaption->colour(getColors()->captionText);

    mDesign = mParentContainer->getGUILayer()->createRectangle(x, y);

    setPosition(x, y);
    setSize(width, height);

    _actualize();
}


Caption::~Caption()
{
    mParentContainer->getGUILayer()->destroyCaption(mCaption);
    mParentContainer->getGUILayer()->destroyRectangle(mDesign);
}


void Caption::injectKeyPressed(const OIS::KeyEvent& evt)
{
}
    

void Caption::injectKeyReleased(const OIS::KeyEvent& evt)
{
}


void Caption::injectKeys(vector<OIS::KeyCode>& keys)
{
}


void Caption::injectMousePressed(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
}


void Caption::injectMouseReleased(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
}


bool Caption::isOver(const Ogre::Vector2& pos)
{
    mOvered = mCaption->intersects(pos);
    return mOvered;
}


void Caption::setSize(int width, int height)
{
    mCaption->size(width, height);
    mDesign->width(width);
    mDesign->height(height);
}


void Caption::setPosition(int left, int top)
{
    mCaption->left(left);
    mCaption->top(top);

    mDesign->left(left);
    mDesign->top(top);
}


Ogre::Vector2 Caption::getPosition()
{
    return mDesign->position();
}


void Caption::injectTimeAndMousePosition(double time, const Ogre::Vector2& pos)
{
}


void Caption::highlight()
{
}


void Caption::text(Ogre::String s)
{
    mCaption->text(s);
}


Ogre::String Caption::text()
{
    return mCaption->text();
}


void Caption::_actualize()
{
    if (!mBackgroundSpriteName.empty())
        mDesign->background_image(mBackgroundSpriteName);
    else
    {
        mDesign->background_gradient(getColors()->captionBackgroundGradientType,
                                     getColors()->captionBackgroundGradientStart,
                                     getColors()->captionBackgroundGradientEnd);
        mDesign->border(getColors()->captionBorderSize, getColors()->captionBorder);
    }
}


void Caption::setBackgroundImage(const Ogre::String& backgroundSpriteName)
{
    _clearRectangleDesign();

    if (backgroundSpriteName.length() == 0 || backgroundSpriteName == "none")
        mBackgroundSpriteName = "";
    else
        mBackgroundSpriteName = backgroundSpriteName;

    _actualize();
}

}