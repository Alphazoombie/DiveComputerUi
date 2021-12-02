/**
 * @author Lucas Balda
 */

#ifndef OPTION_SCREEN_H
#define OPTION_SCREEN_H

#include <lvgl.h>
#include "UISystem.h"
#include "ButtonType.h"
#include "Touch.h"

class OptionScreen 
{
public:
    static lv_obj_t* screenObj;
    static lv_obj_t* buttons[4];
    static int16_t selectionIndex;
    static lv_style_t styleSelected;
    static lv_style_t styleDefault; 
    
    static void setup();
    static void showScreen();
    static void processButtonPress(ButtonType, int index);
};

#endif
