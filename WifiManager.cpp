#include "WifiManager.h"

WiFiServer WifiManager::m_server;
FtpServer WifiManager::m_ftpServer;

bool WifiManager::wifiServerActive = false;

void WifiManager::createAccessPoint(int port, const char* ssid, const char* password)
{    
    m_server = WiFiServer(port);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    Serial.println(WiFi.softAPNetworkID());
    wifiServerActive = true;
    m_ftpServer.begin("diver", "diverpass");
    Serial.println("FTP Server started...");
}

void WifiManager::disconnectAccessPoint()
{
    WiFi.softAPdisconnect(true);
    wifiServerActive = false;
}
