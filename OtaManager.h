#ifndef OTAMANAGER_H
#define OTAMANAGER_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include "WifiManager.h"

#define OTA_ACCESS_DATA_PATH "/otadata.wfd"

class OtaManager
{
public:    
    static void setup();
    static void checkForUpdates();
};

#endif