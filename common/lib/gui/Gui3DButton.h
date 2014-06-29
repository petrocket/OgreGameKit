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

#ifndef Gui3DButton_H
#define Gui3DButton_H



#include "Gorilla.h"
#include "Gui3DContainer.h"
#include "Gui3DPanelElementCallback.h"

#include "Gui3DMemberFunction.h"

namespace Gui3D
{

/*! class. Button
    desc.
        A Button.
*/
class Button : public PanelElementCallback, public RectangleDesigned, public Captioned
{
public:
    Button(Ogre::Real x, 
           Ogre::Real y, 
           size_t width,
           size_t height,
           const Ogre::String& text,
           Container* parentContainer);

    ~Button();

    void injectKeyPressed(const OIS::KeyEvent& evt);
    
    void injectKeyReleased(const OIS::KeyEvent& evt);

    void injectKeys(std::vector<OIS::KeyCode>& keys);

    void injectMousePressed(const OIS::MouseEvent& evt, 
        OIS::MouseButtonID id);

    void injectMouseReleased(const OIS::MouseEvent& evt, 
        OIS::MouseButtonID id);
    
    bool isOver(const Ogre::Vector2& pos);

    void setPosition(int left, int top);
    
    void setSize(int width, int height);

    void setText(const Ogre::String& text);
    
    void injectTimeAndMousePosition(double time, const Ogre::Vector2& pos);
    
    /** \brief Define the callback when the button has been released
    */
    template<typename T>
    void setPressedCallback(T* object, bool(T::*method)(PanelElement*))
    {
        changeCallback(new MemberFunction<T>(object, method));
    }

    void highlight();

    void setActive(bool active);

    bool getActive();

    Ogre::Vector2 getPosition();

    void resetState();

    /** \brief Set the over and not over background to a sprite from the texture atlas.
        note.
            The sprites will be set only if both are filled
            To remove the images pass on "none" or a empty string to at least one of sprite names.
    */
    void setBackgroundImage(const Ogre::String& overSpriteName, 
                            const Ogre::String& notOverSpriteName,
                            const Ogre::String& inactiveSpriteName,
                            const Ogre::String& clickedSpriteName);

protected:
    bool mIsActive; //!< \brief The button can be clicked

    bool mIsClicked; //!< \brief The button is currently clicked
    
    Ogre::String mOverSpriteName;

    Ogre::String mNotOverSpriteName;

    Ogre::String mInactiveSpriteName;

    Ogre::String mClickedSpriteName;

    void _over();

    void _unOver();

    void _inactive();

    void _clicked();

    void _actualize();
};

}

#endif