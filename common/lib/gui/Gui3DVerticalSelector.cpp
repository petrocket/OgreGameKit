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


#include "Gui3DVerticalSelector.h"

namespace Gui3D
{

// Static configuration of elements size
static float VERTICAL_SELECTOR_MARGIN_BOTTOM            = 0.02f;
static float VERTICAL_SELECTOR_MARGIN_LEFT              = 0.02f;
static float VERTICAL_SELECTOR_MARGIN_CAPTIONS_BUTTONS  = 0.02f;
static float VERTICAL_SELECTOR_BUTTON_SIZE              = 0.08f; // button are consider as a square ; their size on X and Y is BUTTON_SIZE * width
static float VERTICAL_SELECTOR_CAPTION_X_SIZE           = 0.86f;

static float VERTICAL_SELECTOR_CAPTION_HEIGHT_SPACING   = 0.05f;

static float VERTICAL_SELECTOR_MARGIN_TOP               = 0.02f;
static float VERTICAL_SELECTOR_CAPTION_Y_SIZE           = 0.15f;

using namespace std;

VerticalSelector::VerticalSelector(Ogre::Real x, 
                                   Ogre::Real y,
                                   size_t width,
                                   size_t height,
                                   const vector<Ogre::String>& items,
                                   unsigned int nbDisplayedElements,
                                   Container* parentContainer)
                                   : MultipleElementsSelector(x, y, width, height, items, parentContainer),
                                     mNumTopVisibleElement(0)
{
    mMaxNumberOfVisibleElements = std::min<unsigned int>(nbDisplayedElements, items.size());

    mPreviousElementsButton->text("-");
    mPreviousElementsButton->setPressedCallback(this, &VerticalSelector::allElementsGoDownCallback);

    mNextElementsButton->text("+");
    mNextElementsButton->setPressedCallback(this, &VerticalSelector::allElementsGoTopCallback);
    
    for (size_t i=0; i < mMaxNumberOfVisibleElements; i++)
        mCaptions.push_back(new Caption(0, 0, 1, 1, "", 
                                        mParentContainer,
                                        Gorilla::TextAlign_Centre,
                                        Gorilla::VerticalAlign_Middle));

    setPosition(x, y);
    setSize(width, height);
}


VerticalSelector::~VerticalSelector()
{
    for (size_t i=0; i < mMaxNumberOfVisibleElements; i++)
        delete mCaptions[i];
}


void VerticalSelector::setSize(int width, int height)
{
    mDesign->width(width);
    mDesign->height(height);

    int x = mDesign->left();
    int y = mDesign->top();

    int decalXButtons = x + 
                        width * (VERTICAL_SELECTOR_MARGIN_LEFT + 
                                 VERTICAL_SELECTOR_MARGIN_CAPTIONS_BUTTONS + 
                                 VERTICAL_SELECTOR_CAPTION_X_SIZE);
    int decalYButtonPreviousElements = y + height * VERTICAL_SELECTOR_MARGIN_TOP;
    int decalYButtonNextElements     = y + height * (1 - VERTICAL_SELECTOR_MARGIN_BOTTOM);
    int sizeXYElementButton          =     width * VERTICAL_SELECTOR_BUTTON_SIZE;
    
    mPreviousElementsButton->setPosition(decalXButtons, decalYButtonPreviousElements);
    mPreviousElementsButton->setSize(sizeXYElementButton, sizeXYElementButton);

    mNextElementsButton->setPosition(decalXButtons, decalYButtonNextElements - sizeXYElementButton);
    mNextElementsButton->setSize(sizeXYElementButton, sizeXYElementButton);

    int captionsXPosition   = x + width * VERTICAL_SELECTOR_MARGIN_LEFT;
    int captionsXSize       =     width * VERTICAL_SELECTOR_CAPTION_X_SIZE;

    //int captionsYSize = height * VERTICAL_SELECTOR_CAPTION_Y_SIZE;
    int captionsYSize = height * (1 - (VERTICAL_SELECTOR_MARGIN_TOP + 
                                       (mMaxNumberOfVisibleElements - 1) * VERTICAL_SELECTOR_CAPTION_HEIGHT_SPACING)) / 
                                 mMaxNumberOfVisibleElements;

    int captionsYPadding        =     height * VERTICAL_SELECTOR_CAPTION_HEIGHT_SPACING;
    int captionsYStartPosition  = y + height * VERTICAL_SELECTOR_MARGIN_TOP;

    for (size_t i=0; i < mMaxNumberOfVisibleElements; i++)
    {
        mCaptions.at(i)->setPosition(captionsXPosition, 
                                     captionsYStartPosition + (captionsYSize + captionsYPadding)*i);
        mCaptions.at(i)->setSize(captionsXSize, captionsYSize);
    }
}


void VerticalSelector::setPosition(int left, int top)
{
    Ogre::Real decalLeft    = left - mDesign->left();
    Ogre::Real decalTop     = top - mDesign->top();

    for (size_t i=0; i < mCaptions.size(); i++)
    {
        Ogre::Vector2 pos = mCaptions[i]->getPosition();
        mCaptions[i]->setPosition(pos.x + decalLeft, pos.y + decalTop);
    }

    MultipleElementsSelector::setPosition(left, top);
}


Ogre::Vector2 VerticalSelector::getPosition()
{
    return mDesign->position();
}


bool VerticalSelector::allElementsGoTopCallback(PanelElement* e)
{
    allElementsGoTop();
    return true;
}


bool VerticalSelector::allElementsGoDownCallback(PanelElement* e)
{
    allElementsGoDown();
    return true;
}


void VerticalSelector::allElementsGoTop()
{
    if (mNumTopVisibleElement == mValues.size() - mMaxNumberOfVisibleElements)
        return;
    mNumTopVisibleElement++;

    _actualize();
}


void VerticalSelector::allElementsGoDown()
{
    if (mNumTopVisibleElement == 0)
        return;
    mNumTopVisibleElement--;

    _actualize();
}

}