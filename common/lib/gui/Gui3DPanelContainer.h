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

#ifndef Gui3DPanelContainer_H
#define Gui3DPanelContainer_H



#include <vector>

#include <OIS.h>

#include "Gorilla.h"

#include "Gui3DContainer.h"

#include "Gui3D.h"
#include "Gui3DPanelElement.h"

#include "Gui3DButton.h"
#include "Gui3DCaption.h"
#include "Gui3DCheckbox.h"
#include "Gui3DCheckboxText.h"
#include "Gui3DCheckboxSprite.h"
#include "Gui3DCombobox.h"
#include "Gui3DInlineSelector.h"
#include "Gui3DListbox.h"
#include "Gui3DProgressBar.h"
#include "Gui3DScrollBar.h"
#include "Gui3DTextZone.h"

namespace Gui3D
{

/*! class. PanelContainer
    desc.
        A container contains a layer and the gui object that created it
*/
class PanelContainer : public Container
{
public:
    PanelContainer(Gui3D* gui, const Ogre::Vector2& size);
    
    ~PanelContainer();

    Gorilla::Rectangle* getBackground() { return mBackground; }
    
    PanelElement* getFocusedElement() { return mFocusedPanelElement; }
    
    /** \brief Give the focus to an element of this panel.
        note.
            If the element isn't contained by this panel, nothing will happened.
    */
    void setFocusedElement(PanelElement* e);

    void injectKeyPressed(const OIS::KeyEvent& evt);

    void injectKeyReleased(const OIS::KeyEvent& evt);

    void injectMousePressed(const OIS::MouseEvent& evt,
                            OIS::MouseButtonID id);

    void injectMouseReleased(const OIS::MouseEvent& evt, 
                             OIS::MouseButtonID id);
    
    void injectTouchPressed(const OIS::MultiTouchEvent& evt);

    /** \brief Inject time to have animations with panel elements, such as
    a flash on and off of a cursor in Textzones
    */
    void injectTime(double time);

    /** \brief UnOver all elements.
    */
    void unOverAllElements();

    /** \brief Hide the mouse pointer of this panel
    */
    void hideInternalMousePointer();

    /** \brief Show the mouse pointer of this panel
    */
    void showInternalMousePointer();

    virtual Button* makeButton(Ogre::Real x, 
                               Ogre::Real y, 
                               size_t width,
                               size_t height,
                               const Ogre::String& text);

    virtual Caption* makeCaption(Ogre::Real x, 
                                 Ogre::Real y,
                                 size_t width,
                                 size_t height,
                                 const Ogre::String& text,
                                 Gorilla::TextAlignment textAlign = Gorilla::TextAlign_Left,
                                 Gorilla::VerticalAlignment verticalAlign = Gorilla::VerticalAlign_Top);

    virtual Checkbox* makeCheckbox(Ogre::Real x,
                                   Ogre::Real y,
                                   size_t width,
                                   size_t height);

    virtual Combobox* makeCombobox(Ogre::Real x, 
                                   Ogre::Real y,
                                   size_t width,
                                   size_t height,
                                   const std::vector<Ogre::String>& items,
                                   unsigned int nbDisplayedElements);

    virtual InlineSelector* makeInlineSelector(Ogre::Real x,
                                               Ogre::Real y,
                                               size_t width,
                                               size_t height,
                                               const std::vector<Ogre::String>& items);

    virtual Listbox* makeListbox(Ogre::Real x, 
                                 Ogre::Real y,
                                 size_t width,
                                 size_t height,
                                 const std::vector<Ogre::String>& items,
                                 unsigned int nbDisplayedElements);

    virtual ProgressBar* makeProgressBar(Ogre::Real x, 
                                         Ogre::Real y,
                                         size_t width,
                                         size_t height);

    virtual ScrollBar* makeScrollBar(Ogre::Real x, 
                                     Ogre::Real y,
                                     size_t width,
                                     size_t height,
                                     Ogre::Real minValue,
                                     Ogre::Real maxValue);

    virtual TextZone* makeTextZone(Ogre::Real x,
                                   Ogre::Real y,
                                   size_t width,
                                   size_t height,
                                   const Ogre::String& text);

    void destroyButton(Button*);
    
    void destroyCaption(Caption*);
    
    void destroyCheckbox(Checkbox*);
    
    void destroyCombobox(Combobox*);
    
    void destroyInlineSelector(InlineSelector*);
    
    void destroyProgressBar(ProgressBar*);
    
    void destroyScrollBar(ScrollBar*);
    
    void destroyTextZone(TextZone*);

protected:
    Ogre::Vector2 mInternalMousePos; //!< \brief The Internal position of the mouse in the panel
    Gorilla::Rectangle* mMousePointer;

    std::vector<PanelElement*> mPanelElements;
    PanelElement* mFocusedPanelElement; //!< \brief The element that actually have the focus

    Gorilla::Rectangle* mBackground;
    Ogre::Vector2 mSize;

    /** \brief Save key pressed when there's no focused element
    If a focusedElement is set before keyReleased, these keys are then sent
    */
    std::vector<OIS::KeyCode> mKeyCodes;

    /** \brief Internal use. Use make[XXX] to add element to this container.
    Add an item to the panel.
    */
    void addItem(PanelElement* item);

    /** \brief Internal use. Use destroy[XXX] to remove an element to this container.
    Remove an item to the panel.
    */
    void removeItem(PanelElement* item);
};

}

#endif