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



#include "Gui3DScreenRenderable2DPanel.h"
#include "Gui3DPanelColors.h"

namespace Gui3D
{

using namespace std;

ScreenRenderable2DPanel::ScreenRenderable2DPanel(Gui3D* gui,
                         Gorilla::ScreenRenderable2D* screen,
                         const Ogre::Vector2& startPosition,
                         const Ogre::Vector2& size,
                         const Ogre::String& atlasName,
                         const Ogre::String& name)
                         : PanelContainer(gui, size), mScreen(screen), mStartPosition(startPosition)
{
    mGUILayer = gui->createLayerScreenRenderable2D(mScreen, name);

    mBackground = mGUILayer->createRectangle(mStartPosition.x, 
                                             mStartPosition.y, 
                                             mSize.x, 
                                             mSize.y);

    if (getPanelColors()->panelBackgroundSpriteName.length() == 0 ||
        getPanelColors()->panelBackgroundSpriteName == "none")
    {
        mBackground->background_gradient(mGui3D->getPanelColors()->panelGradientType,
                                         mGui3D->getPanelColors()->panelGradientColorStart,
                                         mGui3D->getPanelColors()->panelGradientColorEnd);
        mBackground->border(mGui3D->getPanelColors()->panelBorderSize, 
                            mGui3D->getPanelColors()->panelBorder);
    }
    else
        mBackground->background_image(getPanelColors()->panelBackgroundSpriteName);

    // Create an empty mouse pointer which follow the mouse cursor
    mMousePointer = mGUILayer->createRectangle(0, 0, 0, 0);
    showInternalMousePointer();
}


ScreenRenderable2DPanel::~ScreenRenderable2DPanel()
{
    for (size_t i=0; i < mPanelElements.size(); i++)
        delete mPanelElements[i];
    
    mScreen->destroy(mGUILayer);
}


bool ScreenRenderable2DPanel::injectMouseMoved(const Ogre::Real& x, const Ogre::Real& y)
{
    mInternalMousePos = Ogre::Vector2(x, y);
    mMousePointer->position(mInternalMousePos);

    // Let's actualize the "over" for each elements
    for (size_t i=0; i < mPanelElements.size(); i++)
        mPanelElements[i]->isOver(mInternalMousePos);

    return true;
}


void ScreenRenderable2DPanel::setPosition(const Ogre::Vector2& startPosition)
{
    for (size_t i=0; i < mPanelElements.size(); i++)
    {
        Ogre::Vector2 oldPanelElementPosition = mPanelElements[i]->getPosition();
        mPanelElements[i]->setPosition(oldPanelElementPosition.x + (startPosition.x - mStartPosition.x),
                                       oldPanelElementPosition.y + (startPosition.y - mStartPosition.y));
    }
    
    mBackground->position(startPosition);

    mStartPosition = startPosition;
}


Button* ScreenRenderable2DPanel::makeButton(Ogre::Real x,
                                Ogre::Real y, 
                                size_t width,
                                size_t height,
                                const Ogre::String& text)
{
    return PanelContainer::makeButton(mStartPosition.x + x, mStartPosition.y + y, 
                                      width, height, 
                                      text);
}


Caption* ScreenRenderable2DPanel::makeCaption(Ogre::Real x,
                                  Ogre::Real y,
                                  size_t width,
                                  size_t height,
                                  const Ogre::String& text,
                                  Gorilla::TextAlignment textAlign,
                                  Gorilla::VerticalAlignment verticalAlign)
{
    return PanelContainer::makeCaption(mStartPosition.x + x, mStartPosition.y + y, 
                                       width, height, 
                                       text, textAlign, verticalAlign);
}


Checkbox* ScreenRenderable2DPanel::makeCheckbox(Ogre::Real x,
                                    Ogre::Real y,
                                    size_t width,
                                    size_t height)
{
    return PanelContainer::makeCheckbox(mStartPosition.x + x, mStartPosition.y + y, 
                                        width, height);
}


Combobox* ScreenRenderable2DPanel::makeCombobox(Ogre::Real x,
                                    Ogre::Real y,
                                    size_t width,
                                    size_t height,
                                    const vector<Ogre::String>& items,
                                    unsigned int nbDisplayedElements)
{
    return PanelContainer::makeCombobox(mStartPosition.x + x, mStartPosition.y + y, 
                                        width, height, 
                                        items, nbDisplayedElements);
}


Listbox* ScreenRenderable2DPanel::makeListbox(Ogre::Real x,
                                  Ogre::Real y,
                                  size_t width,
                                  size_t height,
                                  const vector<Ogre::String>& items,
                                  unsigned int nbDisplayedElements)
{
    return PanelContainer::makeListbox(mStartPosition.x + x, mStartPosition.y + y, 
                                       width, height, 
                                       items, nbDisplayedElements);
}


InlineSelector* ScreenRenderable2DPanel::makeInlineSelector(Ogre::Real x,
                                                Ogre::Real y,
                                                size_t width,
                                                size_t height,
                                                const vector<Ogre::String>& items)
{
    return PanelContainer::makeInlineSelector(mStartPosition.x + x, mStartPosition.y + y, 
                                              width, height, 
                                              items);
}


ScrollBar* ScreenRenderable2DPanel::makeScrollBar(Ogre::Real x,
                                      Ogre::Real y,
                                      size_t width,
                                      size_t height,
                                      Ogre::Real minValue,
                                      Ogre::Real maxValue)
{
    return PanelContainer::makeScrollBar(mStartPosition.x + x, mStartPosition.y + y, 
                                         width, height, 
                                         minValue, maxValue);
}


ProgressBar* ScreenRenderable2DPanel::makeProgressBar(Ogre::Real x,
                                          Ogre::Real y,
                                          size_t width,
                                          size_t height)
{
    return PanelContainer::makeProgressBar(mStartPosition.x + x, mStartPosition.y + y, 
                                           width, height);
}


TextZone* ScreenRenderable2DPanel::makeTextZone(Ogre::Real x,
                                    Ogre::Real y,
                                    size_t width,
                                    size_t height,
                                    const Ogre::String& text)
{
    return PanelContainer::makeTextZone(mStartPosition.x + x, mStartPosition.y + y, 
                                        width, height, 
                                        text);
}

}