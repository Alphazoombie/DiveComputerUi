#include "WifiManager.h"

WiFiServer WifiManager::m_server;

void WifiManager::createAccessPoint(int port, const char* ssid, const char* password)
{    
    m_server = WiFiServer(port);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
}

void WifiManager::disconnectAccessPoint()
{
    WiFi.softAPdisconnect(true);
}
