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


#include "Gui3DTextZone.h"

namespace Gui3D
{

using namespace std;

TextZone::TextZone(Ogre::Real x, 
                   Ogre::Real y,
                   size_t width,
                   size_t height,
                   const Ogre::String& text, 
                   Container* parentContainer)
                   : PanelElementCallback(parentContainer), stringCursorEdit(text, '|'),
                     showStringWithCursor(false)
{
    mCaption = mParentContainer->getGUILayer()->createCaption(getColors()->textzoneTextSize, x, y, 
                                                              stringCursorEdit.getStringWithoutCursor());
    mCaption->align(Gorilla::TextAlign_Left);
    mCaption->vertical_align(Gorilla::VerticalAlign_Top);
    mCaption->background(getColors()->transparent);
    mCaption->colour(getColors()->textzoneText);

    mDesign = mParentContainer->getGUILayer()->createRectangle(x, y);
    mDesign->border(getColors()->textzoneBorderSize,
                    getColors()->textzoneBorder);
    mDesign->background_gradient(getColors()->textzoneBackgroundNotOveredGradientType,
                                 getColors()->textzoneBackgroundNotOveredGradientStart,
                                 getColors()->textzoneBackgroundNotOveredGradientEnd);

    setPosition(x, y);
    setSize(width, height);

    _actualize();
}


TextZone::~TextZone()
{
    mParentContainer->getGUILayer()->destroyCaption(mCaption);
    mParentContainer->getGUILayer()->destroyRectangle(mDesign);
}


void TextZone::setSize(int width, int height)
{
    mCaption->size(width, height);

    mDesign->width(width);
    mDesign->height(height);
}


void TextZone::setMaxStringLength(int maxLength)
{
    stringCursorEdit.setMaxLength(maxLength);
}


void TextZone::setValue(Ogre::String s)
{
    stringCursorEdit.setText(s);
    mCaption->text(s);
    callCallback();
}


Ogre::String TextZone::getValue()
{
    return stringCursorEdit.getStringWithoutCursor();
}


Ogre::Vector2 TextZone::getPosition()
{
    return mDesign->position();
}


void TextZone::highlight()
{
    mDesign->border_colour(getColors()->textzoneBorderHighlight);
}


bool TextZone::isOver(const Ogre::Vector2& pos)
{
    // Only modify the textZone color if it hasn't the focus
    bool overed = mDesign->intersects(pos);
    if (!mHasFocus)
    {
        if (overed && !mOvered)
            _over();
        else if (!overed && mOvered)
            _unOver();
    }
    mOvered = overed;
    _actualize();
    return mOvered;
}


void TextZone::setPosition(int left, int top)
{
    mCaption->left(left);
    mCaption->top(top);

    mDesign->left(left);
    mDesign->top(top);
}


void TextZone::injectKeyPressed(const OIS::KeyEvent& evt)
{
    if (evt.key == OIS::KC_RIGHT)
        stringCursorEdit.cursorRight();
    else if (evt.key == OIS::KC_LEFT)
        stringCursorEdit.cursorLeft();
    else if (evt.key == OIS::KC_BACK)
        stringCursorEdit.backspace();
    else if (evt.key == OIS::KC_DELETE)
        stringCursorEdit.deleteLetter();
    else
        stringCursorEdit.injectKey(static_cast<char>(evt.text));

    mCaption->text(stringCursorEdit.getStringWithCursor());
}


void TextZone::injectKeys(vector<OIS::KeyCode>& keys)
{
}


void TextZone::injectKeyReleased(const OIS::KeyEvent& evt)
{
    if (evt.key == OIS::KC_RETURN)
        setFocus(false);
}


void TextZone::injectMousePressed(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
}


void TextZone::injectMouseReleased(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
}


void TextZone::injectTimeAndMousePosition(double time, const Ogre::Vector2& pos)
{
    if (mHasFocus)
    {
        // Every 1/2 sec, make the cursor flash on and off
        if (clock.clockAction(0.5, time))
        {
            Ogre::String str;
            str = (showStringWithCursor) ? stringCursorEdit.getStringWithoutCursor() :
                                           stringCursorEdit.getStringWithCursor();
            mCaption->text(str);

            showStringWithCursor = !showStringWithCursor;
        }
    }
}


void TextZone::_over()
{
    mDesign->background_gradient(getColors()->textzoneBackgroundNotOveredGradientType,
                                 getColors()->textzoneBackgroundOveredGradientStart,
                                 getColors()->textzoneBackgroundOveredGradientEnd);
}


void TextZone::_unOver()
{
    mDesign->background_gradient(getColors()->textzoneBackgroundNotOveredGradientType,
                                 getColors()->textzoneBackgroundNotOveredGradientStart,
                                 getColors()->textzoneBackgroundNotOveredGradientEnd);
}


void TextZone::_focused()
{
    mDesign->border_colour(getColors()->textzoneBorderSelected);
    mDesign->background_gradient(getColors()->textzoneBackgroundSelectedGradientType,
                                 getColors()->textzoneBackgroundSelectedGradientStart,
                                 getColors()->textzoneBackgroundSelectedGradientEnd);
}


void TextZone::_actualize()
{
    if (mHasFocus)
        _focused();
    else
    {
        mOvered ? _over() : _unOver();

        // Make sur that when we're not focusing the textZone, the
        //  text cursor isn't visible anymore
        mCaption->text(stringCursorEdit.getStringWithoutCursor());
        mDesign->border_colour(getColors()->textzoneBorder);

        callCallback();
    }
}

}