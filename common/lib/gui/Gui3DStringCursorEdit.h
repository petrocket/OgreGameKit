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

#ifndef Gui3DStringCursorEdit_H
#define Gui3DStringCursorEdit_H



#include "OgreString.h"

namespace Gui3D
{

/*! class. StringCursorEdit
    desc.
        This class manage a string with a cursor
*/
class StringCursorEdit
{
public:
    StringCursorEdit();

    StringCursorEdit(Ogre::String, char cursor);
    
    ~StringCursorEdit();
    
    /** \brief Return the string without the cursor symbol
    */
    Ogre::String getStringWithoutCursor();
    
    /** \brief Return the string with the cursor symbol
    */
    Ogre::String getStringWithCursor();
    
    void injectKey(char key);
    
    /** \brief Move the cursor to the right in the string
    */
    void cursorRight();
    
    /** Move the cursor to the left in the string
    */
    void cursorLeft();
    
    /** \brief Erase the current text and replace it with a new one.
    The cursor position is set at the end of the new text
    */
    void setText(const Ogre::String text);

    /** \brief Set the cursor symbol
    */
    void setCursor(char cursor);
    
    /** \brief Remove a letter which is at the left of the cursor and
    move the cursor to the left (as in all OS input)
    */
    void backspace();

    /** \brief Remove a letter which is a the right of the cursor
    */
    void deleteLetter();

    /** \brief Set the maximum length of the string
    */
    void setMaxLength(int maxLength);

protected:
    Ogre::String mStr; //!< \brief The string itself

    unsigned short int mCursorPosition; //!< \brief The cursor position in the string
    
    short int mMaxLength;

    char mCursor; //!< \brief The character that represents the cursors
};

}

#endif