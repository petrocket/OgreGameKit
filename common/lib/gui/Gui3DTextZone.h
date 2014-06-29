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

#ifndef Gui3DTextZone_H
#define Gui3DTextZone_H



#include <vector>

#include "Gui3DClock.h"

#include "Gorilla.h"
#include "Gui3DPanelElementCallback.h"

#include "Gui3DStringCursorEdit.h"

#include "Gui3DMemberFunction.h"

namespace Gui3D
{

/*! class. PanelElement
    desc.
        An editable text zone.
*/
class TextZone : public PanelElementCallback, public RectangleDesigned, public Captioned
{
public:
    TextZone(Ogre::Real x,
             Ogre::Real y,
             size_t width,
             size_t height,
             const Ogre::String& text,
             Container* parentContainer);

    ~TextZone();
    
    void injectKeyPressed(const OIS::KeyEvent& evt);
    
    void injectKeyReleased(const OIS::KeyEvent& evt);

    void injectKeys(std::vector<OIS::KeyCode>& keys);
    
    void injectMousePressed(const OIS::MouseEvent& evt, 
                            OIS::MouseButtonID id);
    
    void injectMouseReleased(const OIS::MouseEvent& evt, 
                             OIS::MouseButtonID id);

    Ogre::String getValue();

    Ogre::Vector2 getPosition();

    void highlight();
    
    bool isOver(const Ogre::Vector2& pos);

    void injectTimeAndMousePosition(double time, const Ogre::Vector2& pos);

    /** \brief Set the max string length of the textZone
    */
    void setMaxStringLength(int maxLength);

    /** \brief Set the value of the text zone
    */
    void setValue(Ogre::String s);

    void setSize(int width, int height);

    void setPosition(int left, int top);

    /** \brief Define the callback when the string has been validated
    */
    template<typename T>
    void setValueChangedCallback(T* object, bool(T::*method)(PanelElement*))
    {
        changeCallback(new MemberFunction<T>(object, method));
    }

protected:
    Clock clock; //!< \brief Internal use for animation of the string

    bool showStringWithCursor;    //!< \brief Cursor with the string or not

    StringCursorEdit stringCursorEdit;    //!< \brief The text zone string

    void _over();

    void _unOver();

    void _focused();

    void _actualize();
};

}

#endif