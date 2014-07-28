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

#ifndef Gui3DPanelElement_H
#define Gui3DPanelElement_H



#include <vector>

#include <Ogre.h>
#include <OIS.h>

#include "Gorilla.h"

#include "Gui3DContainer.h"

namespace Gui3D
{

/*! class. PanelElement
    desc.
        Interface for every Gorilla Panel Elements
*/
class PanelElement
{
public:
    PanelElement(Container* parentContainer);

    ~PanelElement();

    /** \brief Inject a key pressed. The element can loose the focus.
    */
    virtual void injectKeyPressed(const OIS::KeyEvent& evt) = 0;

    /** \brief Inject a key released. The element can loose the focus.
    */
    virtual void injectKeyReleased(const OIS::KeyEvent& evt) = 0;

    /** \brief Inject a mouse pressed. The element can loose the focus.
    */
    virtual void injectMousePressed(const OIS::MouseEvent& evt, 
                                    OIS::MouseButtonID id) = 0;

    /** \brief Inject a touch pressed. The element can loose the focus.
     */
    virtual void injectTouchPressed(const OIS::MultiTouchEvent& evt) = 0;
    
    /** \brief Inject a mouse released. The element can loose the focus.
    */
    virtual void injectMouseReleased(const OIS::MouseEvent& evt, 
                                     OIS::MouseButtonID id) = 0;

    /** \brief Inject actual pressed key (Usefull when some keys were pressed
    when the element get the focus). ex : press CTRL before click on a Listbox for multiple selection.
    The first element of the passed vector is the first pressed element, and so on.
    */
    virtual void injectKeys(std::vector<OIS::KeyCode>& keys) = 0;

    /** \brief Get if the element is overed
    */
    bool getOvered();

    /** \brief Get if the element has the focus
    */
    bool getFocus();

    /** \brief Get the element's position
    */
    virtual Ogre::Vector2 getPosition() = 0;

    /** \brief Tests if the element isOver by something at the position pos
    */
    virtual bool isOver(const Ogre::Vector2& pos) = 0;

    /** \brief Set if the element has the focus
    */
    void setFocus(bool hasFocus);

    /** \brief Set if the element should be overed
    */
    void setOvered(bool overed);

    /** \brief Define the position of the element
    */
    virtual void setPosition(int left, int top) = 0;

    /** \brief Define the size of the element
    */
    virtual void setSize(int width, int height) = 0;

    /** \brief Inject the time and the mouse position
    */
    virtual void injectTimeAndMousePosition(double time, const Ogre::Vector2& pos) = 0;

    /** \brief Reset the state of the element
    */
    virtual void resetState();

    /** \brief Highlight this element
    */
    virtual void highlight() = 0;

protected:
    bool mHasFocus;
    bool mOvered;
    Container* mParentContainer; //!< \brief Pointer to the layer that created this element

    /** \brief Internal use to get the gui panel colors
    */
    PanelColors* getColors();

    /** \brief Internal Use. Actualize the design of the element depending of its state.
    */
    virtual void _actualize() = 0;
};


/** class. RectangleDesigned
    desc.
        Feature of a PanelElement that will be designed by a rectangle.
*/
class RectangleDesigned
{
public:
    RectangleDesigned()
        : mDesign(NULL)
    {
    }

    /** \brief /!\ Be careful when using this. Undesired behaviour
    can happened. You should then just use it to set some
    alignment/colors options.
    Return the main design element of the element (see element
    methods to acces to each others designs of the element).
    */
    Gorilla::Rectangle* getRectangleDesign()
    {
        return mDesign;
    }

protected:
    Gorilla::Rectangle* mDesign; //!< \brief The design of the element
    
    //! \brief Internal use. Clear the rectangle.
    void _clearRectangleDesign()
    {
        mDesign->background_colour(Ogre::ColourValue::White);
    }
};


/** class. Captioned
    desc.
        Feature of a PanelElement that will have a caption.
*/
class Captioned
{
public:
    Captioned()
        : mCaption(NULL)
    {
    }

    /** \brief /!\ Be careful when using this. Undesired behaviour
    can happened. You should then just use it to set some
    alignment/colors options.
    Return the main caption of this element.
    */
    Gorilla::Caption* getCaption()
    {
        return mCaption;
    }

    /** \brief Set the text of this caption
    */
    void text(const Ogre::String& text)
    {
        mCaption->text(text);
    }

    /** \brief Get the text of this caption
    */
    Ogre::String text()
    {
        return mCaption->text();
    }

    /** \brief Set the background of this caption
    */
    void background(const Ogre::ColourValue& background)
    {
        mCaption->background(background);
    }

    /** \brief Set the text colour of this caption
    */
    void textColor(const Ogre::ColourValue& textColor)
    {
        mCaption->colour(textColor);
    }

protected:
    Gorilla::Caption* mCaption; //!< \brief The text container of the element
};

}

#endif