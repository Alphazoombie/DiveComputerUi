#include <Arduino.h>
#include <iostream>
#include <SD.h>
#include <string>
#include <fstream>
#include "WifiAccessGenerator.h"

#define dateLength 10
using namespace std;

class FileSystem
{
public:
    char* m_directoryPath PROGMEM = "/logFiles";
    const char* m_divePath PROGMEM = "/dive.log";
    const char* m_datePath PROGMEM = "/date.log";
    const char* m_sessionsPath PROGMEM = "/sessions.log";
    const char* m_wifiDataPath PROGMEM = "/wifidata.wfd";
    static int m_diveID;
    // char m_date[9];
    // char m_oldDate[9];
    String m_date;
    String m_oldDate;
    char m_logPath[25] PROGMEM;
    bool m_sameSession = false;
    void setDateToFile(); 
    void getDateFromFile(); //char* result
    int  getDiveID(); 
    void setDiveID();
    void writeDateToSessionFile(char* date); 
    void setDate(); 
    void initializeMetaData();
    void createDirectory();  
    bool fileExists();
    void createWifiDataFile();
};
