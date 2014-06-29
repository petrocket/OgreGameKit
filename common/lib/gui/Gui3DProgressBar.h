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

#ifndef Gui3DProgressBar_H
#define Gui3DProgressBar_H



#include <vector>

#include "Gorilla.h"
#include "Gui3DContainer.h"

#include "Gui3DPanelElementCallback.h"

#include "Gui3DMemberFunction.h"

namespace Gui3D
{

/*! class. ProgressBar
    desc.
        A progress bar for loading
*/
class ProgressBar : public PanelElementCallback, public RectangleDesigned, public Captioned
{
public:
    ProgressBar(Ogre::Real x, 
                Ogre::Real y,
                size_t width,
                size_t height,
                Container* parentContainer);
    
    ~ProgressBar();

    void injectKeyPressed(const OIS::KeyEvent& evt);
    
    void injectKeyReleased(const OIS::KeyEvent& evt);
    
    void injectKeys(std::vector<OIS::KeyCode>& keys);

    void injectMousePressed(const OIS::MouseEvent& evt, 
                            OIS::MouseButtonID id);
    
    void injectMouseReleased(const OIS::MouseEvent& evt, 
                             OIS::MouseButtonID id);
    
    bool isOver(const Ogre::Vector2& pos);
    
    void injectTimeAndMousePosition(double time, const Ogre::Vector2& pos);

    void setSize(int width, int height);

    /** \brief The value has to be between [0-1]
    */
    void setValue(double value);

    void setPosition(int left, int top);

    void highlight();

    /** \brief Define the callback when the value has changed
    */
    template<typename T>
    void setValueChangedCallback(T* object, bool(T::*method)(PanelElement*))
    {
        changeCallback(new MemberFunction<T>(object, method));
    }

    void reset();

    double getValue();

    Ogre::Vector2 getPosition();

protected:
    double mValue;

    Gorilla::Rectangle* mProgressBarRectangle;

    void _actualize();
};

}

#endif