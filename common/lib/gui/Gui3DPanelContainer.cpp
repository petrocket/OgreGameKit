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


#include "Gui3DPanelContainer.h"

namespace Gui3D
{

using namespace std;

PanelContainer::PanelContainer(Gui3D* gui, const Ogre::Vector2& size)
    : Container(gui), mSize(size), mFocusedPanelElement(NULL), 
      mMousePointer(NULL), mBackground(NULL)
{
}


PanelContainer::~PanelContainer()
{
}


void PanelContainer::setFocusedElement(PanelElement* e)
{
    for (size_t i=0; i < mPanelElements.size(); i++)
    {
        if (mPanelElements[i] == e)
        {
            // Unfocus the previous focused element
            if (mFocusedPanelElement != e)
                mFocusedPanelElement->setFocus(false);

            e->setFocus(true);
            mFocusedPanelElement = e;
            return;
        }
    }
}


void PanelContainer::addItem(PanelElement* item)
{
    mPanelElements.push_back(item);
}


void PanelContainer::removeItem(PanelElement* item)
{
    vector<PanelElement*>::iterator it = 
        mPanelElements.begin();
    while (it != mPanelElements.end())
    {
        if (*it == item)
        {
            mPanelElements.erase(it);
            return;
        }
        ++it;
    }
}


void PanelContainer::injectKeyPressed(const OIS::KeyEvent& evt)
{
    if (mFocusedPanelElement == NULL)
    {
        // Save the pressed key to eventually send to the futur focusedElement
        mKeyCodes.push_back(evt.key);
        return;
    }
    else
    {
        mFocusedPanelElement->injectKeyPressed(evt);
        if (!mFocusedPanelElement->getFocus())
            mFocusedPanelElement = NULL;
    }
}


void PanelContainer::injectKeyReleased(const OIS::KeyEvent& evt)
{
    if (mFocusedPanelElement == NULL)
    {
        // Remove the key from the vector
        vector<OIS::KeyCode>::iterator it = mKeyCodes.begin();
        while (it != mKeyCodes.end())
        {
            if (*it == evt.key)
            {
                mKeyCodes.erase(it);
                return;
            }
            ++it;
        }
    }
    else
    {
        mFocusedPanelElement->injectKeyReleased(evt);
        if (!mFocusedPanelElement->getFocus())
            mFocusedPanelElement = NULL;
    }
}


void PanelContainer::injectMousePressed(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
    bool found = false;
    
    // Try to find the element that has been clicked
    for (size_t i=0; i < mPanelElements.size(); i++)
    {
        if (mPanelElements[i]->isOver(mInternalMousePos))
        {
            if (mFocusedPanelElement != NULL &&
                mFocusedPanelElement != mPanelElements[i])
                mFocusedPanelElement->setFocus(false);

            mFocusedPanelElement = mPanelElements[i];
            mFocusedPanelElement->setFocus(true);
            found = true;
            break;
        }
    }

    // It's a click on the panel.
    if (!found)
    {
        if (mFocusedPanelElement != NULL)
        {
            mFocusedPanelElement->setFocus(false);
            mFocusedPanelElement = NULL;
        }
        return;
    }

    // Send key to the new focused Elements
    mFocusedPanelElement->injectKeys(mKeyCodes);
    mKeyCodes.clear();
    mFocusedPanelElement->injectMousePressed(evt, id);
    if (!mFocusedPanelElement->getFocus())
        mFocusedPanelElement = NULL;
}

void PanelContainer::injectTouchPressed(const OIS::MultiTouchEvent& evt)
{
    bool found = false;
    
    mInternalMousePos = Ogre::Vector2(evt.state.X.abs, evt.state.Y.abs);
    
    // Try to find the element that has been clicked
    for (size_t i=0; i < mPanelElements.size(); i++)
    {
        if (mPanelElements[i]->isOver(mInternalMousePos))
        {
            if (mFocusedPanelElement != NULL &&
                mFocusedPanelElement != mPanelElements[i])
                mFocusedPanelElement->setFocus(false);
            
            mFocusedPanelElement = mPanelElements[i];
            mFocusedPanelElement->setFocus(true);
            found = true;
            break;
        }
    }
    
    // It's a click on the panel.
    if (!found)
    {
        if (mFocusedPanelElement != NULL)
        {
            mFocusedPanelElement->setFocus(false);
            mFocusedPanelElement = NULL;
        }
        return;
    }
    
    // Send key to the new focused Elements
    mFocusedPanelElement->injectKeys(mKeyCodes);
    mKeyCodes.clear();
    mFocusedPanelElement->injectTouchPressed(evt);
    if (!mFocusedPanelElement->getFocus())
        mFocusedPanelElement = NULL;
}

void PanelContainer::injectMouseReleased(const OIS::MouseEvent& evt, 
    OIS::MouseButtonID id)
{
    if (mFocusedPanelElement == NULL)
        return;

    mFocusedPanelElement->injectMouseReleased(evt, id);
    if (!mFocusedPanelElement->getFocus())
        mFocusedPanelElement = NULL;
}


void PanelContainer::unOverAllElements()
{
    // UnOver all panelElement by putting the mouse outside of 
    //  the panel and actualize the over for each elements
    mInternalMousePos = Ogre::Vector2(-1, -1);
    for (size_t i=0; i < mPanelElements.size(); i++)
        mPanelElements[i]->isOver(mInternalMousePos);

    if (mFocusedPanelElement != NULL)
    {
        mFocusedPanelElement->resetState();
        mFocusedPanelElement->setFocus(false);
        mFocusedPanelElement = NULL;
    }
}


void PanelContainer::injectTime(double time)
{
    for (size_t i=0; i < mPanelElements.size(); i++)
        mPanelElements[i]->injectTimeAndMousePosition(time, mInternalMousePos);
}


void PanelContainer::hideInternalMousePointer()
{
    if (mMousePointer == NULL)
        return;
    
    mMousePointer->background_image(NULL);

    // Set background image as NULL isnt enought and still display a rectangle.
    // Set it's size to 0 then.
    mMousePointer->width(0);
    mMousePointer->height(0);
}


void PanelContainer::showInternalMousePointer()
{
    if (mMousePointer == NULL)
        return;

    if (getPanelColors()->panelCursorSpriteName.length() == 0 ||
        getPanelColors()->panelCursorSpriteName == "none")
    {
        mMousePointer->background_colour(Gorilla::Colours::None);
    }
    else
    {
        mMousePointer->width(getPanelColors()->panelCursorSpriteSizeX);
        mMousePointer->height(getPanelColors()->panelCursorSpriteSizeY);
        mMousePointer->background_image(getPanelColors()->panelCursorSpriteName);
    }
}


Button* PanelContainer::makeButton(Ogre::Real x, 
                                   Ogre::Real y, 
                                   size_t width,
                                   size_t height,
                                   const Ogre::String& text)
{
    Button* button = new Button(x, y, width, height, text, this);
    button->setBackgroundImage(getPanelColors()->buttonOveredSpriteName,
                               getPanelColors()->buttonNotOveredSpriteName,
                               getPanelColors()->buttonInactiveSpriteName,
                               getPanelColors()->buttonClickedSpriteName);
    addItem(button);
    return button;
}


Caption* PanelContainer::makeCaption(Ogre::Real x, 
                                     Ogre::Real y,
                                     size_t width,
                                     size_t height,
                                     const Ogre::String& text,
                                     Gorilla::TextAlignment textAlign,
                                     Gorilla::VerticalAlignment verticalAlign)
{
    Caption* caption = new Caption(x, y, width, height, text, this, textAlign, verticalAlign);
    caption->setBackgroundImage(getPanelColors()->captionBackgroundSpriteName);
    addItem(caption);
    return caption;
}


Checkbox* PanelContainer::makeCheckbox(Ogre::Real x, 
                                       Ogre::Real y,
                                       size_t width,
                                       size_t height)
{
    Checkbox* checkbox = NULL;

    // If images weren't defined, use the text checkbox
    if (getPanelColors()->checkboxCheckedNotOveredBackgroundSpriteName.empty())
        checkbox = new CheckboxText(x, y, width, height, 
                                    getPanelColors()->checkboxCheckedSymbol,
                                    this);
    else
        checkbox = new CheckboxSprite(x, y, width, height, 
                                      getPanelColors()->checkboxCheckedNotOveredBackgroundSpriteName, 
                                      getPanelColors()->checkboxCheckedOveredBackgroundSpriteName,
                                      getPanelColors()->checkboxOveredBackgroundSpriteName, 
                                      getPanelColors()->checkboxNotOveredBackgroundSpriteName, 
                                      this);
    addItem(checkbox);
    return checkbox;
}


Combobox* PanelContainer::makeCombobox(Ogre::Real x, 
                                       Ogre::Real y,
                                       size_t width,
                                       size_t height,
                                       const vector<Ogre::String>& items,
                                       unsigned int nbDisplayedElements)
{
    Combobox* combobox = new Combobox(x, y, width, height, items, nbDisplayedElements, this);
    combobox->setBackgroundImageButtons(getPanelColors()->comboboxButtonPreviousOveredSpriteName,
                                        getPanelColors()->comboboxButtonPreviousNotOveredSpriteName,
                                        getPanelColors()->comboboxButtonPreviousInactiveSpriteName,
                                        getPanelColors()->comboboxButtonPreviousClickedSpriteName,
                                        getPanelColors()->comboboxButtonNextOveredSpriteName,
                                        getPanelColors()->comboboxButtonNextNotOveredSpriteName,
                                        getPanelColors()->comboboxButtonNextInactiveSpriteName,
                                        getPanelColors()->comboboxButtonNextClickedSpriteName);
    addItem(combobox);
    return combobox;
}


Listbox* PanelContainer::makeListbox(Ogre::Real x, 
                                     Ogre::Real y,
                                     size_t width,
                                     size_t height,
                                     const vector<Ogre::String>& items,
                                     unsigned int nbDisplayedElements)
{
    Listbox* listBox = new Listbox(x, y, width, height, items, nbDisplayedElements, this);
    listBox->setBackgroundImageButtons(getPanelColors()->listboxButtonPreviousOveredSpriteName,
                                       getPanelColors()->listboxButtonPreviousNotOveredSpriteName,
                                       getPanelColors()->listboxButtonPreviousInactiveSpriteName,
                                       getPanelColors()->listboxButtonPreviousClickedSpriteName,
                                       getPanelColors()->listboxButtonNextOveredSpriteName,
                                       getPanelColors()->listboxButtonNextNotOveredSpriteName,
                                       getPanelColors()->listboxButtonNextInactiveSpriteName,
                                       getPanelColors()->listboxButtonNextClickedSpriteName);
    addItem(listBox);
    return listBox;
}


InlineSelector* PanelContainer::makeInlineSelector(Ogre::Real x,
                                                   Ogre::Real y,
                                                   size_t width,
                                                   size_t height,
                                                   const vector<Ogre::String>& items)
{
    InlineSelector* inlineSelector = new InlineSelector(x, y, width, height, items, this);
    inlineSelector->setBackgroundImageButtons(getPanelColors()->inlineselectorButtonPreviousOveredSpriteName,
                                              getPanelColors()->inlineselectorButtonPreviousNotOveredSpriteName,
                                              getPanelColors()->inlineselectorButtonPreviousInactiveSpriteName,
                                              getPanelColors()->inlineselectorButtonPreviousClickedSpriteName,
                                              getPanelColors()->inlineselectorButtonNextOveredSpriteName,
                                              getPanelColors()->inlineselectorButtonNextNotOveredSpriteName,
                                              getPanelColors()->inlineselectorButtonNextInactiveSpriteName,
                                              getPanelColors()->inlineselectorButtonNextClickedSpriteName);
    addItem(inlineSelector);
    return inlineSelector;
}


ScrollBar* PanelContainer::makeScrollBar(Ogre::Real x, 
                                         Ogre::Real y,
                                         size_t width,
                                         size_t height,
                                         Ogre::Real minValue,
                                         Ogre::Real maxValue)
{
    ScrollBar* scrollBar = new ScrollBar(x, y, width, height, minValue, maxValue, this);
    scrollBar->setCursorSprites(getPanelColors()->scrollbarCursorOveredSpriteName,
                                getPanelColors()->scrollbarCursorNotOveredSpriteName,
                                getPanelColors()->scrollbarCursorSelectedSpriteName);
    addItem(scrollBar);
    return scrollBar;
}


ProgressBar* PanelContainer::makeProgressBar(Ogre::Real x, 
                                             Ogre::Real y,
                                             size_t width,
                                             size_t height)
{
    ProgressBar* progressBar = new ProgressBar(x, y, width, height, this);
    addItem(progressBar);
    return progressBar;
}


TextZone* PanelContainer::makeTextZone(Ogre::Real x,
                                       Ogre::Real y,
                                       size_t width,
                                       size_t height,
                                       const Ogre::String& text)
{
    TextZone* textZone = new TextZone(x, y, width, height, text, this);
    addItem(textZone);
    return textZone;
}


void PanelContainer::destroyButton(Button* b)
{
    removeItem(b);
}
    

void PanelContainer::destroyCaption(Caption* c)
{
    removeItem(c);
}
    

void PanelContainer::destroyCheckbox(Checkbox* c)
{
    removeItem(c);
}
    

void PanelContainer::destroyCombobox(Combobox* c)
{
    removeItem(c);
}
    

void PanelContainer::destroyInlineSelector(InlineSelector* is)
{
    removeItem(is);
}
    

void PanelContainer::destroyProgressBar(ProgressBar* p)
{
    removeItem(p);
}
    

void PanelContainer::destroyScrollBar(ScrollBar* s)
{
    removeItem(s);
}
    

void PanelContainer::destroyTextZone(TextZone* t)
{
    removeItem(t);
}

}