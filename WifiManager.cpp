#include "WifiManager.h"

WiFiServer WifiManager::m_server;
bool WifiManager::serverActive;
bool WifiManager::clientActive;
bool WifiManager::connectionFailed = false;
FtpServer WifiManager::m_ftpServer;
bool WifiManager::wifiServerActive = false;

void WifiManager::createAccessPoint(int port, const char* ssid, const char* password)
{    
    if (clientActive)
    {
        disconnectAccessPoint();
    }

    m_server = WiFiServer(port);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
	
    serverActive = true;
    Serial.println(WiFi.softAPNetworkID());
    wifiServerActive = true;
    m_ftpServer.begin("diver", "diverpass");
    Serial.println("FTP Server started...");
}

void WifiManager::disconnectAccessPoint()
{
    if (serverActive == true)
    {
        WiFi.softAPdisconnect(true);
        serverActive = false;
        serverActive = false;		
		wifiServerActive = false;
    }
    
    if (clientActive == true)
    {
        WiFi.disconnect();        
    } 
}

bool WifiManager::connectAccessPoint(char* ssid, char* password)
{
    if (serverActive)
    {
        disconnectAccessPoint();
    }
    int sizeSsid = 0;
    int sizePass = 0;
    for (size_t i = 0; i < 29; i++)
    {         
        if (ssid[i] == '\0' || ssid[i] == '\n')
        {
            break;
        }        
        sizeSsid++;  
    }

    for (size_t i = 0; i < 29; i++)
    {               
        if (password[i] == '\0' || password[i] == '\n')
        {
            break;
        }
        sizePass++;          
    }

    char realSsid[sizeSsid];
    char realPass[sizePass];

    for (size_t i = 0; i < sizeSsid; i++)
    {
        realSsid[i] = ssid[i];
    }
    realSsid[sizeSsid] =  '\0';

    for (size_t i = 0; i < sizePass; i++)
    {
        realPass[i] = password[i];
    }
    realPass[sizePass] =  '\0';

    Serial.println("ssid pass:");
    Serial.println(realSsid);
    Serial.println(realPass);
    int counter = 0;
    Serial.println(counter);
    WiFi.begin(realSsid, realPass);
    while (WiFi.status() != WL_CONNECTED && counter < 10) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    counter++;
    }
    if (counter >= 10)
    {
        Serial.println("Connection failed");
        connectionFailed = true;        
        return false;
    }
    else    
    {
        connectionFailed = false;
        return true;
    }   
}

bool WifiManager::isConnected()
{
    return (WiFi.status() == WL_CONNECTED);
}