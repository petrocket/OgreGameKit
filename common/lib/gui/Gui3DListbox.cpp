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


#include "Gui3DListbox.h"

namespace Gui3D
{

using namespace std;

Listbox::Listbox(Ogre::Real x, 
                 Ogre::Real y,
                 size_t width,
                 size_t height,
                 const vector<Ogre::String>& items,
                 unsigned int nbDisplayedElements,
                 Container* parentContainer)
                 : VerticalSelector(x, y, width, height, items, nbDisplayedElements, parentContainer),
                   addedMode(false)
{
    mDesign->background_gradient(getColors()->listboxBackgroundGradientType,
                                 getColors()->listboxBackgroundGradientStart,
                                 getColors()->listboxBackgroundGradientEnd);
    mDesign->border(getColors()->listboxBorderSize,
                    getColors()->listboxBorder);

    _actualize();

    // init
    for (size_t i=0; i < items.size(); i++)
        mSelectedElements[i] = false;

    for (size_t i=0; i < mMaxNumberOfVisibleElements; i++)
    {
        mCaptions[i]->textColor(getColors()->listboxText);
        mCaptions[i]->getCaption()->font(getColors()->listboxTextSize);
    }
}


Listbox::~Listbox()
{
}


void Listbox::injectKeyPressed(const OIS::KeyEvent& evt)
{
    if (evt.key == OIS::KC_LCONTROL ||
        evt.key == OIS::KC_RCONTROL)
    {
        addedMode = true;
    }
}


void Listbox::injectKeyReleased(const OIS::KeyEvent& evt)
{
    if (evt.key == OIS::KC_LCONTROL ||
        evt.key == OIS::KC_RCONTROL)
    {
        addedMode = false;
    }
}


void Listbox::injectKeys(vector<OIS::KeyCode>& keys)
{
    for (size_t i=0; i < keys.size(); i++)
    {
        if (keys.at(i) == OIS::KC_LCONTROL ||
            keys.at(i) == OIS::KC_RCONTROL)
        {
            addedMode = true;
        }
    }
}


void Listbox::injectMousePressed(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
    VerticalSelector::injectMousePressed(evt, id);

    if (id == OIS::MB_Left)
    {
        // Actualize selected elements if the overed element is set
        if (mActualOveredElement >= 0)
        {
            // AddedMode is set when the user has control button down
            if (!addedMode)
            {
                for (size_t i=0; i < mValues.size(); i++)
                    mSelectedElements[i] = false;
            }

            mSelectedElements[mActualOveredElement + mNumTopVisibleElement] = 
                !mSelectedElements[mActualOveredElement + mNumTopVisibleElement];

            // The selected element can't be overed
            mActualOveredElement = MULTIPLE_ELEMENT_SELECTOR_NO_ELEMENT;
            _actualize();
            callCallback();
        }
    }
}


bool Listbox::isOver(const Ogre::Vector2& pos)
{
    if (VerticalSelector::isOver(pos))
        return true;

    // Consider the cursor over something
    mOvered = true;

    // Consider there's no elements overed
    if (mActualOveredElement >= 0)
    {
        if (mSelectedElements[mNumTopVisibleElement + mActualOveredElement])
        {
            mCaptions[mActualOveredElement]
                ->background(getColors()->listboxNotOveredSelectedElement);
        }
        else
        {
            mCaptions[mActualOveredElement]
                ->background(getColors()->listboxNotOveredElement);
        }
    }

    mActualOveredElement = MULTIPLE_ELEMENT_SELECTOR_NO_ELEMENT;

    for (size_t i=0; i < mCaptions.size(); i++)
    {
        if (mCaptions[i]->isOver(pos))
        {
            mActualOveredElement = i;
            // Is it a selected element ?
            if (mSelectedElements[mNumTopVisibleElement + i])
            {
                mCaptions[mActualOveredElement]
                    ->background(getColors()->listboxOveredSelectedElement);
            }
            else
            {
                mCaptions[mActualOveredElement]
                    ->background(getColors()->listboxOveredElement);
            }
            return true;
        }
    }

    // mOvered now depends on design intersects
    mOvered = mDesign->intersects(pos);
    // We don't _actualize() because this function take care of
    //  actualizing the captions elements instead of _actualize() (optimisation)
    return mOvered;
}


void Listbox::setCurrentValues(vector<Ogre::String>& itemsName)
{
    for (size_t i=0; i < itemsName.size(); i++)
    {
        for (size_t j=0; j < mValues.size(); j++)
        {
            if (itemsName.at(i).compare(mValues.at(j))==0)
            {
                mSelectedElements[j] = true;
                break;
            }
        }
    }

    _actualize();
    callCallback();
}


void Listbox::setCurrentValue(Ogre::String& itemName)
{
    vector<Ogre::String> v;
    v.push_back(itemName);
    setCurrentValues(v);
}


vector<Ogre::String> Listbox::getValues()
{
    vector<Ogre::String> values;
    map<int, bool>::iterator it = mSelectedElements.begin();
    while (it != mSelectedElements.end())
    {
        if ((*it).second)
            values.push_back(mValues[(*it).first]);
        ++it;
    }
    return values;
}


void Listbox::_actualize()
{
    for (size_t i = 0; i < mCaptions.size(); i++)
    {
        Ogre::ColourValue color;

        unsigned int actualElementIndice = i + mNumTopVisibleElement;
        if (mSelectedElements[actualElementIndice])
            color = getColors()->listboxNotOveredSelectedElement;
        else if (i == mActualOveredElement)
            color = getColors()->listboxOveredElement;
        else
            color = getColors()->listboxNotOveredElement;
        
        mCaptions[i]->text(mValues[actualElementIndice]);
        mCaptions[i]->background(color);
    }
}


void Listbox::setFocus(bool hasFocus)
{
    VerticalSelector::setFocus(hasFocus);
    if (!mHasFocus)
        addedMode = false;
}


void Listbox::highlight()
{
    mDesign->border_colour(getColors()->listboxBorderHighlight);
}

}