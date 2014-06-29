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


#include "Gui3DMultipleElementsSelector.h"

namespace Gui3D
{

MultipleElementsSelector::MultipleElementsSelector(Ogre::Real x, 
                                                   Ogre::Real y,
                                                   size_t width,
                                                   size_t height,
                                                   const std::vector<Ogre::String>& items,
                                                   Container* parentContainer)
                                                   : PanelElementCallback(parentContainer), 
                                                      mActualOveredElement(MULTIPLE_ELEMENT_SELECTOR_NO_ELEMENT)
{
    mDesign = mParentContainer->getGUILayer()->createRectangle(x, y, width, height);

    mPreviousElementsButton = new Button(0, 0, 30, 30, "", mParentContainer);
    mNextElementsButton = new Button(0, 0, 30, 30, "", mParentContainer);

    for (size_t i = 0; i < items.size(); i++)
        addItem(items[i]);
}


MultipleElementsSelector::~MultipleElementsSelector()
{
    mParentContainer->getGUILayer()->destroyRectangle(mDesign);

    delete mNextElementsButton;
    delete mPreviousElementsButton;
}


void MultipleElementsSelector::injectMousePressed(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left)
    {
        if (mActualOveredElement == MULTIPLE_ELEMENT_SELECTOR_PREVIOUS_ELEMENT_BUTTON)
        {
            mPreviousElementsButton->injectMousePressed(evt, id);
            return;
        }
        else if (mActualOveredElement == MULTIPLE_ELEMENT_SELECTOR_NEXT_ELEMENT_BUTTON)
        {
            mNextElementsButton->injectMousePressed(evt, id);
            return;
        }
    }
}


void MultipleElementsSelector::injectMouseReleased(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left)
    {
        if (mActualOveredElement == MULTIPLE_ELEMENT_SELECTOR_PREVIOUS_ELEMENT_BUTTON)
            mPreviousElementsButton->injectMouseReleased(evt, id);
        else if (mActualOveredElement == MULTIPLE_ELEMENT_SELECTOR_NEXT_ELEMENT_BUTTON)
            mNextElementsButton->injectMouseReleased(evt, id);
    }
}


bool MultipleElementsSelector::isOver(const Ogre::Vector2& pos)
{
    // As the buttons are independant of the rest, first check
    //  if they are overed.
    if (mPreviousElementsButton->isOver(pos))
    {
        mActualOveredElement = MULTIPLE_ELEMENT_SELECTOR_PREVIOUS_ELEMENT_BUTTON;
        return true;
    }
    else if (mNextElementsButton->isOver(pos))
    {
        mActualOveredElement = MULTIPLE_ELEMENT_SELECTOR_NEXT_ELEMENT_BUTTON;
        return true;
    }

    return false;
}


void MultipleElementsSelector::addItem(Ogre::String itemName)
{
    mValues.push_back(itemName);
}


void MultipleElementsSelector::injectTimeAndMousePosition(double time, const Ogre::Vector2& pos)
{
}


void MultipleElementsSelector::resetState()
{
    mPreviousElementsButton->resetState();
    mNextElementsButton->resetState();

    PanelElement::resetState();
}


void MultipleElementsSelector::setBackgroundImageButtons(const Ogre::String& previousButtonOveredSpriteName,
                                                         const Ogre::String& previousButtonNotOveredSpriteName,
                                                         const Ogre::String& previousButtonInactiveSpriteName,
                                                         const Ogre::String& previousButtonClickedSpriteName,
                                                         const Ogre::String& nextButtonOveredSpriteName,
                                                         const Ogre::String& nextButtonNotOveredSpriteName,
                                                         const Ogre::String& nextButtonInactiveSpriteName,
                                                         const Ogre::String& nextButtonClickedSpriteName)
{
    // Make sure buttons background images are not empty or none
    //  to remove the text on buttons
    if (previousButtonOveredSpriteName.length() == 0    || previousButtonOveredSpriteName == "none"     ||
        previousButtonNotOveredSpriteName.length() == 0 || previousButtonNotOveredSpriteName == "none"  ||
        previousButtonInactiveSpriteName.length() == 0  || previousButtonInactiveSpriteName == "none"   ||
        previousButtonClickedSpriteName.length() == 0   || previousButtonClickedSpriteName == "none"    ||
        nextButtonOveredSpriteName.length() == 0        || nextButtonOveredSpriteName == "none"         ||
        nextButtonNotOveredSpriteName.length() == 0     || nextButtonNotOveredSpriteName == "none"      ||
        nextButtonInactiveSpriteName.length() == 0      || nextButtonInactiveSpriteName == "none"       ||
        nextButtonClickedSpriteName.length() == 0       || nextButtonClickedSpriteName == "none")
    {
        mPreviousElementsButton->setText("<");
        mNextElementsButton->setText(">");
    }
    else
    {
        mPreviousElementsButton->setBackgroundImage(previousButtonOveredSpriteName, 
                                                    previousButtonNotOveredSpriteName, 
                                                    previousButtonInactiveSpriteName,
                                                    previousButtonClickedSpriteName);
        mPreviousElementsButton->setText("");

        mNextElementsButton->setBackgroundImage(nextButtonOveredSpriteName, 
                                                nextButtonNotOveredSpriteName, 
                                                nextButtonInactiveSpriteName,
                                                nextButtonClickedSpriteName);
        mNextElementsButton->setText("");
    }
}


void MultipleElementsSelector::setPosition(int left, int top)
{
    Ogre::Real decalLeft = left - mDesign->left();
    Ogre::Real decalTop = top - mDesign->top();

    mDesign->left(mDesign->left()+decalLeft);
    mDesign->top(mDesign->top()+decalTop);

    Ogre::Vector2 pButtonPos = mPreviousElementsButton->getPosition() + Ogre::Vector2(decalLeft, decalTop);
    mPreviousElementsButton->setPosition(pButtonPos.x, pButtonPos.y);

    Ogre::Vector2 nButtonPos = mNextElementsButton->getPosition() + Ogre::Vector2(decalLeft, decalTop);
    mNextElementsButton->setPosition(nButtonPos.x, nButtonPos.y);
}


Button* MultipleElementsSelector::getPreviousElementsButton()
{
    return mPreviousElementsButton;
}


Button* MultipleElementsSelector::getNextElementsButton()
{
    return mNextElementsButton;
}

}