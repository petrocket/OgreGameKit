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


#include "Gui3DProgressBar.h"

#include <algorithm>

using namespace std;

namespace Gui3D
{

using namespace std;

ProgressBar::ProgressBar(Ogre::Real x, 
                         Ogre::Real y,
                         size_t width,
                         size_t height,
                         Container* parentContainer)
                         : PanelElementCallback(parentContainer)
{
    mCaption = mParentContainer->getGUILayer()->createCaption(getColors()->progressbarTextSize, x, y, "");
    mCaption->align(Gorilla::TextAlign_Centre);
    mCaption->vertical_align(Gorilla::VerticalAlign_Middle);
    mCaption->background(getColors()->transparent);
    mCaption->colour(getColors()->progressbarText);

    mDesign = mParentContainer->getGUILayer()->createRectangle(x, y);
    mDesign->border(getColors()->progressbarBorderSize,
                    getColors()->progressbarBorder);
    mDesign->background_gradient(getColors()->progressbarBackgroundGradientType,
                                 getColors()->progressbarBackgroundGradientStart,
                                 getColors()->progressbarBackgroundGradientEnd);

    mProgressBarRectangle = mParentContainer->getGUILayer()->createRectangle(x, y);
    mProgressBarRectangle->background_gradient(getColors()->progressbarLoadingBarGradientType,
                                               getColors()->progressbarLoadingBarGradientStart,
                                               getColors()->progressbarLoadingBarGradientEnd);
    
    setPosition(x, y);
    setSize(width, height);

    setValue(0);

    _actualize();
}


ProgressBar::~ProgressBar()
{
    mParentContainer->getGUILayer()->destroyCaption(mCaption);
    mParentContainer->getGUILayer()->destroyRectangle(mDesign);

    mParentContainer->getGUILayer()->destroyRectangle(mProgressBarRectangle);
}


void ProgressBar::injectKeyPressed(const OIS::KeyEvent& evt)
{
}


void ProgressBar::injectKeyReleased(const OIS::KeyEvent& evt)
{
}


void ProgressBar::injectKeys(vector<OIS::KeyCode>& keys)
{
}


void ProgressBar::injectMousePressed(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
}


void ProgressBar::injectMouseReleased(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
}


bool ProgressBar::isOver(const Ogre::Vector2& pos)
{
    mOvered = mDesign->intersects(pos);
    _actualize();
    return mOvered;
}


void ProgressBar::injectTimeAndMousePosition(double time, const Ogre::Vector2& pos)
{
}


void ProgressBar::setPosition(int left, int top)
{
    mCaption->left(left);
    mCaption->top(top);

    mDesign->left(left);
    mDesign->top(top);

    mProgressBarRectangle->left(left);
    mProgressBarRectangle->top(top);
}


void ProgressBar::highlight()
{
    mDesign->border_colour(getColors()->progressbarBorderHighlight);
}


void ProgressBar::setSize(int width, int height)
{
    mCaption->size(width, height);
    
    mDesign->width(width);
    mDesign->height(height);
    
    mProgressBarRectangle->width(mValue * width);
    mProgressBarRectangle->height(height);
}


void ProgressBar::setValue(double value)
{
    // Make sure value is between 0 and 1
    mValue = max<double>(0, min<double>(1, value)); 

    int newProgressBarRectangleWidth = mValue * mCaption->width();

    mProgressBarRectangle->width(newProgressBarRectangleWidth);

    _actualize();

    callCallback();
}


void ProgressBar::reset()
{
    mValue = 0;
    mProgressBarRectangle->width(mValue);
    _actualize();
}


double ProgressBar::getValue()
{
    return mValue;
}


Ogre::Vector2 ProgressBar::getPosition()
{
    return mDesign->position();
}


void ProgressBar::_actualize()
{
    ostringstream s;
    s << fixed << setprecision(0) << (mValue * 100) << "%";
    mCaption->text(s.str());
}

}