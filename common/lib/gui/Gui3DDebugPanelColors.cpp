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


#include "Gui3DDebugPanelColors.h"

namespace Gui3D
{

DebugPanelColors::DebugPanelColors()
    : PanelColors()
{
    transparent = Gorilla::rgb(0, 0, 0, 0);
    Ogre::ColourValue red = Gorilla::rgb(255, 0, 0, 255);
    Ogre::ColourValue notOvered = Gorilla::rgb(220, 140, 255, 200);
    Ogre::ColourValue overed = Gorilla::rgb(200, 90, 255, 200);
    Ogre::ColourValue selected = Gorilla::rgb(190, 50, 255, 200);
    Ogre::ColourValue inactive = Gorilla::rgb(230, 190, 255, 200);
    Ogre::ColourValue grey = Gorilla::rgb(150, 150, 150, 255);
    int debugBorderSize = 1;

    // Panel
    panelGradientColorStart = grey;
    panelGradientColorEnd = grey;
    panelBorder = red;
    panelBorderSize = debugBorderSize;

    // Button
    buttonBackgroundOveredGradientStart = overed;
    buttonBackgroundOveredGradientEnd = overed;

    buttonBackgroundNotOveredGradientStart = notOvered;
    buttonBackgroundNotOveredGradientEnd = notOvered;

    buttonBackgroundInactiveGradientStart = inactive;
    buttonBackgroundInactiveGradientEnd = inactive;

    buttonText = Gorilla::rgb(255, 255, 255, 255);
    buttonTextInactive = Gorilla::rgb(255, 255, 255, 70);
    buttonTextSize = 14;

    buttonBorder = red;
    buttonBorderHighlight = red;
    buttonBorderInactive = red;
    buttonBorderSize = debugBorderSize;

    // Checkbox
    checkboxCheckedSymbol = 'X';

    checkboxOveredGradientStart = overed;
    checkboxOveredGradientEnd = overed;
    
    checkboxNotOveredGradientStart = notOvered;
    checkboxNotOveredGradientEnd = notOvered;

    checkboxText = Ogre::ColourValue::White;
    checkboxTextSize = 14;
    
    checkboxBorder = red;
    checkboxBorderHighlight = red;
    checkboxBorderSize = debugBorderSize;

    // Combobox
    comboboxBackgroundGradientStart = Gorilla::rgb(255, 255, 255, 0);
    comboboxBackgroundGradientEnd = Gorilla::rgb(255, 255, 255, 0);

    comboboxOveredElement = overed;
    comboboxNotOveredElement = notOvered;
    comboboxSelectedElement = selected;

    comboboxText = Ogre::ColourValue::White;
    comboboxTextSize = 14;

    comboboxBorder = red;
    comboboxBorderHighlight = red;
    comboboxBorderSize = debugBorderSize;

    // InlineSelector
    inlineselectorText = Ogre::ColourValue::White;
    inlineselectorTextSize = 14;

    inlineselectorBackgroundGradientStart = notOvered;
    inlineselectorBackgroundGradientEnd = notOvered;

    inlineselectorBorder = red;
    inlineselectorBorderHighlight = red;
    inlineselectorBorderSize = debugBorderSize;

    // Listbox
    // same as combobox
    listboxBackgroundGradientStart = comboboxBackgroundGradientStart;
    listboxBackgroundGradientEnd = comboboxBackgroundGradientEnd;

    listboxOveredElement = overed;
    listboxNotOveredElement = notOvered;
    listboxOveredSelectedElement = selected;
    listboxNotOveredSelectedElement = selected;

    listboxText = comboboxText;
    listboxTextSize = comboboxTextSize;

    listboxBorder = red;
    listboxBorderHighlight = red;
    listboxBorderSize = debugBorderSize;

    // Scrollbar
    scrollbarBackgroundGradientStart = notOvered;
    scrollbarBackgroundGradientEnd = notOvered;

    scrollbarProgressbarGradientStart = overed;
    scrollbarProgressbarGradientEnd = overed;

    scrollbarCursorOveredGradientStart = overed;
    scrollbarCursorOveredGradientEnd = overed;

    scrollbarCursorNotOveredGradientStart = notOvered;
    scrollbarCursorNotOveredGradientEnd = notOvered;

    scrollbarCursorSelectedGradientStart = selected;
    scrollbarCursorSelectedGradientEnd = selected;

    scrollbarText = Ogre::ColourValue::White;
    scrollbarTextSize = 14;

    scrollbarBorder = red;
    scrollbarBorderHighlight = red;
    scrollbarCursorBorder =  red;
    scrollbarBorderSize = debugBorderSize;
    scrollbarCursorBorderSize = debugBorderSize;

    // TextZone
    textzoneBackgroundOveredGradientStart = overed;
    textzoneBackgroundOveredGradientEnd = overed;
    
    textzoneBackgroundNotOveredGradientStart = notOvered;
    textzoneBackgroundNotOveredGradientEnd = notOvered;
    
    textzoneBackgroundSelectedGradientStart = selected;
    textzoneBackgroundSelectedGradientEnd = selected;

    textzoneText = Ogre::ColourValue::White;
    textzoneTextSize = 14;
    
    textzoneBorder = red;
    textzoneBorderHighlight = red;
    textzoneBorderSelected = red;
    textzoneBorderSize = debugBorderSize;

    // Caption
    captionBackgroundGradientStart = transparent;
    captionBackgroundGradientEnd = transparent;
    captionBorder = red;
    captionText = Ogre::ColourValue::White;
    captionTextSize = 14;
    captionBorderSize = debugBorderSize;

    // ProgressBar
    progressbarBackgroundGradientStart = notOvered;
    progressbarBackgroundGradientEnd = notOvered;
    
    progressbarLoadingBarGradientStart = selected;
    progressbarLoadingBarGradientEnd = selected;
    
    progressbarText = Ogre::ColourValue::White;
    progressbarTextSize = 14;
    
    progressbarBorder = red;
    progressbarBorderHighlight = red;
    progressbarBorderSize = debugBorderSize;
}

}