#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <Arduino.h>
#include <WiFiClient.h>
#include "ESP32FtpServer.h"

class WifiManager
{
public:
    static WiFiServer m_server;
    static FtpServer m_ftpServer;
    static bool wifiServerActive;
    
    static void createAccessPoint(int port, const char* ssid, const char* password);
    static bool connectAccessPoint(char* ssid, char* password);
    static void disconnectAccessPoint();
    static bool isConnected();
    static bool serverActive;
    static bool clientActive;
    static bool connectionFailed;
};

#endif
