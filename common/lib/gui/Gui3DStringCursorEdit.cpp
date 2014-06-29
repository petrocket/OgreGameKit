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


#include "Gui3DStringCursorEdit.h"

namespace Gui3D
{

using namespace std;

StringCursorEdit::StringCursorEdit() 
    : mStr(""), mCursorPosition(0), mCursor('|'), mMaxLength(-1)
{
}


StringCursorEdit::StringCursorEdit(string s, char separator) 
    : mStr(s), mCursorPosition(s.size()), mCursor(separator), mMaxLength(-1)
{
}


StringCursorEdit::~StringCursorEdit(void)
{
}


string StringCursorEdit::getStringWithoutCursor()
{
    return mStr;
}


string StringCursorEdit::getStringWithCursor()
{
    string strToReturn;
    strToReturn = mStr.substr(0, mCursorPosition);
    strToReturn += mCursor;
    strToReturn.append(mStr.substr(mCursorPosition, mStr.size()));
    return strToReturn;
}


void StringCursorEdit::injectKey(char key)
{
    // Don't inject anything if the max length is reached
    if (mMaxLength != -1 && 
        mStr.size() == mMaxLength)
        return;

    int asciiKey = static_cast<int>(key);
    
    // Only accept [0-9a-zA-Z_-]
    if ((48 <= asciiKey && asciiKey <= 57) || // 0-9
        asciiKey == 45 || // -
        asciiKey == 95 || // _
        (65 <= asciiKey && asciiKey <= 90) || // A-Z
        (97 <= asciiKey && asciiKey <= 122) || // a-z
        asciiKey == 32) // [space]
        {
            string newStr;
            newStr = mStr.substr(0, mCursorPosition);
            newStr.append(&key, 1);
            newStr.append(mStr.substr(mCursorPosition, mStr.size()));
            mStr = newStr;
    
            // Default comportement in all OS : after adding a letter, 
            //  move the cursor to the right
            cursorRight();
        }
}


void StringCursorEdit::cursorRight()
{
    if (mCursorPosition < mStr.size())
        mCursorPosition++;
}


void StringCursorEdit::cursorLeft()
{
    if (mCursorPosition > 0)
        mCursorPosition--;
}


void StringCursorEdit::setText(const string text)
{
    mStr = text;
    mCursorPosition = mStr.size();
}


void StringCursorEdit::setCursor(char cursor)
{
    mCursor = cursor;
}


void StringCursorEdit::backspace()
{
    // Can't backspace at the begging of the text
    if (mCursorPosition == 0)
        return;

    string newStr;
    // Remove the letter before the mCursorPosition
    newStr = mStr.substr(0, mCursorPosition - 1);
    newStr.append(mStr.substr(mCursorPosition, mStr.size()));
    mStr = newStr;

    cursorLeft();
}


void StringCursorEdit::deleteLetter()
{
    // Can't backspace at the end of the text
    if (mCursorPosition == mStr.size())
        return;

    string newStr;
    // Remove the letter after the mCursorPosition
    newStr = mStr.substr(0, mCursorPosition);
    newStr.append(mStr.substr(mCursorPosition + 1, mStr.size()));
    mStr = newStr;
}


void StringCursorEdit::setMaxLength(int maxLength)
{
    mMaxLength = maxLength;
}

}