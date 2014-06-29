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


#include "Gui3DCombobox.h"

namespace Gui3D
{

using namespace std;

Combobox::Combobox(Ogre::Real x, 
                   Ogre::Real y,
                   size_t width,
                   size_t height,
                   const vector<Ogre::String>& items,
                   unsigned int nbDisplayedElements,
                   Container* parentContainer)
                   : VerticalSelector(x, y, width, height, items, nbDisplayedElements, parentContainer), 
                     mActualSelectedElement(0)
{
    mDesign->background_gradient(getColors()->comboboxBackgroundGradientType,
                                 getColors()->comboboxBackgroundGradientStart,
                                 getColors()->comboboxBackgroundGradientEnd);
    mDesign->border(getColors()->comboboxBorderSize,
                    getColors()->comboboxBorder);

    for (size_t i=0; i < mMaxNumberOfVisibleElements; i++)
    {
        mCaptions[i]->textColor(getColors()->comboboxText);
        mCaptions[i]->getCaption()->font(getColors()->comboboxTextSize);
    }

    _actualize();
}


Combobox::~Combobox()
{
}


void Combobox::injectKeyPressed(const OIS::KeyEvent& evt)
{
}


void Combobox::injectKeyReleased(const OIS::KeyEvent& evt)
{
}


void Combobox::injectKeys(vector<OIS::KeyCode>& keys)
{
}


void Combobox::injectMousePressed(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
    VerticalSelector::injectMousePressed(evt, id);

    if (id == OIS::MB_Left)
    {
        // Actualize the selected element if the overed element is set
        if (mActualOveredElement >= 0)
        {
            mActualSelectedElement = mActualOveredElement + mNumTopVisibleElement;
            // The selected element can't be overed
            mActualOveredElement = MULTIPLE_ELEMENT_SELECTOR_NO_ELEMENT;
            _actualize();
            callCallback();
        }
    }
}


bool Combobox::isOver(const Ogre::Vector2& pos)
{
    if (VerticalSelector::isOver(pos))
        return true;

    // Consider the cursor over something
    mOvered = true;

    // Consider nothing is overed
    int save_actualOveredElement = mActualOveredElement;
    if (save_actualOveredElement >= 0)
        mCaptions[save_actualOveredElement]->background(getColors()->comboboxNotOveredElement);

    mActualOveredElement = MULTIPLE_ELEMENT_SELECTOR_NO_ELEMENT;

    for (int i=0; i < mCaptions.size(); i++)
    {
        // Don't need to do anything for the selected element
        if (i == mActualSelectedElement - mNumTopVisibleElement)
            continue;

        if (mCaptions[i]->isOver(pos))
        {
            mActualOveredElement = i;
            mCaptions[mActualOveredElement]->background(getColors()->comboboxOveredElement);
            return true;
        }
    }

    // mOvered now depends on design intersects
    mOvered = mDesign->intersects(pos);
    // We don't _actualize() because this function take care of
    //  actualizing the captions elements instead of _actualize() (optimisation)
    return mOvered;
}


void Combobox::setCurrentValue(Ogre::String& itemName)
{
    for (size_t i=0; i < mCaptions.size(); i++)
    {
        if (mValues[i].compare(itemName) == 0)
        {
            mActualSelectedElement = i;
            _actualize();
            return;
        }
    }
}


Ogre::String Combobox::getValue()
{
    return mValues[mActualSelectedElement];
}


void Combobox::_actualize()
{
    for (size_t i = 0; i < mCaptions.size(); i++)
    {
        Ogre::ColourValue color;

        unsigned int actualElementIndice = i + mNumTopVisibleElement;
        if (actualElementIndice == mActualSelectedElement)
            color = getColors()->comboboxSelectedElement;
        else if (i == mActualOveredElement)
            color = getColors()->comboboxOveredElement;
        else
            color = getColors()->comboboxNotOveredElement;
        
        mCaptions[i]->text(mValues[actualElementIndice]);
        mCaptions[i]->background(color);
    }
}


void Combobox::highlight()
{
    mDesign->border_colour(getColors()->comboboxBorderHighlight);
}

}