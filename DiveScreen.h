/**
 * @author Lucas Balda
 */

#ifndef DIVE_SCREEN_H
#define DIVE_SCREEN_H

#include <lvgl.h>
#include "UISystem.h"
#include "ButtonType.h"
#include "Touch.h"

class DiveScreen 
{
public:
    static lv_obj_t* screenObj;
    static lv_obj_t* lblTimeObj;
    static lv_obj_t* lblSpO2Obj;
    static lv_obj_t* barDepthObj;
    static lv_obj_t* arcSpO2Obj;
    static lv_obj_t* imgHeartFrequencyObj;
    static lv_obj_t* imgTimeObj;
    static lv_obj_t* imgSpO2Obj;
    static lv_obj_t* imgDepthObj;
    static lv_obj_t* lblDepth;
    static lv_obj_t* lblHeartFrequency;
    static void setup();
    static void showScreen();
    static void dataUpdate();
    static void update();
};

#endif
