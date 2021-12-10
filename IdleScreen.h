/**
 * @author Lucas Balda
 */

#ifndef IDLE_SCREEN_H
#define IDLE_SCREEN_H

#include <lvgl.h>
#include "UISystem.h"
#include "ButtonType.h"
#include "Touch.h"

class IdleScreen 
{
public:
    static lv_obj_t* screenObj;
    static lv_obj_t* lblTime;  
     
    static void setup();
    static void showScreen();
    static void processButtonPress(ButtonType);
    static void dataUpdate();
    static void update();
};

#endif
