#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include <Arduino.h>
#include <SD.h>
#include <lvgl.h>
#include <vector>
#include "UISystem.h"
#include "ButtonType.h"
#include "Helper.h"
#include "OtaManager.h"

#define OTA_ACCESS_DATA_PATH "/ota_data.wfd"
class SettingsScreen 
{
public:
    static lv_obj_t* screenObj;
    static lv_obj_t* buttons[3];
    static lv_obj_t* btnOtaData;
    static lv_obj_t* btnOta;
    static lv_obj_t* btnSettingsBack;

    static int16_t selectionIndex;

    static lv_style_t styleSelected;
    static lv_style_t styleDefault;

    static bool otaActive;    
    
    static void setup();
    static void showScreen();
    static void processButtonPress(ButtonType, int index);
    static void dataUpdate();
    static void update();
    static void readWifiAccessData(char* ssid, char* password);
    static bool createsampledata();
    static void buildWifiStatusString(char* ssid, char* status, char* output);
};

#endif
