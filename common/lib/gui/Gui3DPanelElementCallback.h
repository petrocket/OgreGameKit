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

#ifndef Gui3DPanelElementCallback_H
#define Gui3DPanelElementCallback_H



#include "Gui3DPanelElement.h"
#include "Gui3DFunctorBase.h"

namespace Gui3D
{

/*! class. PanelElementCallback
    desc.
        Interface for all Gui3DPanelElements that contains a callback
*/
class PanelElementCallback : public PanelElement
{
public:
    PanelElementCallback(Container* parentContainer)
    : PanelElement(parentContainer), mCallback(NULL)
    {}

protected:
    FunctorBase* mCallback;

    /** \brief Call the element callback
    */
    void callCallback()
    {
        if (mCallback != NULL)
            (*mCallback)(this);
    }

    /** \brief Change the callback of the element
    */
    void changeCallback(FunctorBase* newCallback)
    {
        if (mCallback != NULL)
            delete mCallback;
        mCallback = newCallback;
    }
};

}

#endif