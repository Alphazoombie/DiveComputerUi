#ifndef WIFI_SCREEN_H
#define WIFI_SCREEN_H

#include <Arduino.h>
#include <SD.h>
#include <lvgl.h>
#include <vector>
#include "UISystem.h"
#include "ButtonType.h"
#include "WifiAccessGenerator.h"
#include "WifiManager.h"
#include "Helper.h"


#define WIFI_ACCESS_DATA_PATH "/wifidata.wfd"

class WifiScreen 
{
public:
    static lv_obj_t* screenObj;
    static lv_obj_t* buttons[4];
    static lv_obj_t* labelWifi;
    static lv_obj_t* labelWifiAccessData;
    static lv_obj_t* labelWifiGenerate;

    static int16_t selectionIndex;

    static lv_style_t styleSelected;
    static lv_style_t styleDefault;

    static bool wifiActive;    
    
    static void setup();
    static void showScreen();
    static void processButtonPress(ButtonType);
    static void dataUpdate();
    static void update();
    static void readWifiAccessData(char* ssid, char* password);
    static bool saveWifiAccessData(char* ssid, char* password);
    static void buildWifiAccessDataString(char* ssid, char* password, char* output);
};

#endif
