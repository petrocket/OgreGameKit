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

#ifndef Gui3DContainer_H
#define Gui3DContainer_H


#include "Gorilla.h"

#include "Gui3D.h"

namespace Gui3D
{

/*! class. Container
    desc.
        A container contains a layer and the gui object that created it
*/
class Container
{
public:
    Container(Gui3D* gui);
    
    ~Container();

    /** \brief Return the Gui3D layer
    */
    Gorilla::Layer* getGUILayer();

    /** \brief Return the Gui3D Panel colors of the Gui3D that created it
    */
    PanelColors* getPanelColors();

protected:
    Gorilla::Layer* mGUILayer;

    Gui3D* mGui3D; //!< \brief Pointer to the Gui3D that created it
};

}

#endif