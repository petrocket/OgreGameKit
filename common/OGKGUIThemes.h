//
//  OGKGUIThemes.h
//  OgreGameKit
//
//  Created by Alex on 7/3/14.
//
//

#ifndef OgreGameKit_OGKGUIThemes_h
#define OgreGameKit_OGKGUIThemes_h

#include "Gui3DPanelColors.h"

// DEFAULT THEME
class OGKDefaultGUITheme : public Gui3D::PanelColors
{
public:
    inline OGKDefaultGUITheme():Gui3D::PanelColors()
    {
        transparent = Gorilla::rgb(0,0,0,0);
        
        // Panel
        //        panelBackgroundSpriteName = "panelBackground";
        panelCursorSpriteName = "mousepointer";
        panelCursorSpriteSizeX = 15;
        panelCursorSpriteSizeY = 22;
        panelGradientType = Gorilla::Gradient_NorthSouth;
        panelGradientColorStart = Gorilla::rgb(255, 255, 255, 15);
        panelGradientColorEnd = Gorilla::rgb(255, 255, 255, 45);
        
        // Button
        //        buttonInactiveSpriteName = "buttonInactive";
        //        buttonOveredSpriteName = "buttonOvered";
        //        buttonNotOveredSpriteName = "buttonNotOvered";
        //        buttonClickedSpriteName = "buttonClicked";
        
        buttonBackgroundClickedGradientType = Gorilla::Gradient_NorthSouth;
        buttonBackgroundClickedGradientStart = Gorilla::rgb(255, 255, 255, 200);
        buttonBackgroundClickedGradientEnd = Gorilla::rgb(255, 255, 255, 170);
        
        buttonBackgroundOveredGradientType = Gorilla::Gradient_NorthSouth;
        buttonBackgroundOveredGradientStart = Gorilla::rgb(255, 255, 255, 128);
        buttonBackgroundOveredGradientEnd = Gorilla::rgb(255, 255, 255, 100);
        
        buttonBackgroundNotOveredGradientType = Gorilla::Gradient_NorthSouth;
        buttonBackgroundNotOveredGradientStart = Gorilla::rgb(255, 255, 255, 80);
        buttonBackgroundNotOveredGradientEnd = Gorilla::rgb(255, 255, 255, 50);
        
        buttonBackgroundInactiveGradientType = Gorilla::Gradient_NorthSouth;
        buttonBackgroundInactiveGradientStart = Gorilla::rgb(255, 255, 255, 15);
        buttonBackgroundInactiveGradientEnd = Gorilla::rgb(255, 255, 255, 5);
        
        buttonText = Gorilla::rgb(255, 255, 255, 255);
        buttonTextInactive = Gorilla::rgb(255, 255, 255, 70);
        buttonTextSize = 14;
        
        // @TODO add other button styles
        
        // Caption
        captionBackgroundGradientType = Gorilla::Gradient_NorthSouth;
        captionBackgroundGradientStart = transparent;
        captionBackgroundGradientEnd = transparent;
        
        captionBorder = transparent;
        captionText = Ogre::ColourValue::White;
        captionTextSize = 14;
        captionBorderSize = 0;
        
        // Checkbox
        checkboxTextSize = 14;
        checkboxCheckedSymbol = 'x';
        checkboxBorderSize = 0;
        
        checkboxSelectedOveredGradientType = Gorilla::Gradient_NorthSouth;
        checkboxSelectedOveredGradientStart = Gorilla::rgb(255, 255, 255, 200);
        checkboxSelectedOveredGradientEnd = Gorilla::rgb(255, 255, 255, 170);
        
        checkboxOveredGradientType = Gorilla::Gradient_NorthSouth;
        checkboxOveredGradientStart = Gorilla::rgb(255, 255, 255, 128);
        checkboxOveredGradientEnd = Gorilla::rgb(255, 255, 255, 100);
        
        checkboxSelectedNotOveredGradientType = Gorilla::Gradient_NorthSouth;
        checkboxSelectedNotOveredGradientStart = Gorilla::rgb(255, 255, 255, 80);
        checkboxSelectedNotOveredGradientEnd = Gorilla::rgb(255, 255, 255, 50);
        
        checkboxNotOveredGradientType = Gorilla::Gradient_NorthSouth;
        checkboxNotOveredGradientStart = Gorilla::rgb(255, 255, 255, 15);
        checkboxNotOveredGradientEnd = Gorilla::rgb(255, 255, 255, 5);
        
        // Combobox
        comboboxTextSize = 14;
        comboboxBackgroundGradientType = Gorilla::Gradient_NorthSouth;
        comboboxBackgroundGradientStart = Gorilla::rgb(255, 255, 255, 15);
        comboboxBackgroundGradientEnd = Gorilla::rgb(255, 255, 255, 5);
        
        comboboxOveredElement = Gorilla::rgb(255, 255, 255, 10);
        comboboxNotOveredElement = Gorilla::rgb(255, 255, 255, 5);
        comboboxSelectedElement = Gorilla::rgb(255, 255, 255, 15);
        
        comboboxButtonPreviousOveredSpriteName = "comboboxpreviouselementbuttonovered";
        comboboxButtonPreviousNotOveredSpriteName = "comboboxpreviouselementbuttonnotovered";
        comboboxButtonPreviousInactiveSpriteName = "comboboxpreviouselementbuttoninactive";
        comboboxButtonPreviousClickedSpriteName = "comboboxpreviouselementbuttonnotovered";
        
        comboboxButtonNextOveredSpriteName = "comboboxnextelementbuttonovered";
        comboboxButtonNextNotOveredSpriteName = "comboboxnextelementbuttonnotovered";
        comboboxButtonNextInactiveSpriteName = "comboboxnextelementbuttoninactive";
        comboboxButtonNextClickedSpriteName = "comboboxnextelementbuttonovered";

    }
};

#endif
