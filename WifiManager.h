#ifndef WIFIMANAGER_H
#define WIFIMANGER_H

#include <WiFi.h>
#include <Arduino.h>
#include <WiFiClient.h>

class WifiManager
{
public:
    static WiFiServer m_server;
    static void createAccessPoint(int port, const char* ssid, const char* password);
    static void disconnectAccessPoint();
};

#endif
