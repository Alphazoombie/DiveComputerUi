#ifndef WIFI_SCREEN_H
#define WIFI_SCREEN_H

#include <lvgl.h>
#include "UISystem.h"
#include "ButtonType.h"

class WifiScreen {
    public:
        static lv_obj_t* screenObj;
        static lv_obj_t* buttons[4];
        static lv_obj_t* labelWifi;
        static int16_t selectionIndex;
        static lv_style_t styleSelected;
        static lv_style_t styleDefault;
        static bool wifiActive;

        static void setup();
        static void showScreen();
        static void processButtonPress(ButtonType);
        static void dataUpdate();
        static void update();
};

#endif
