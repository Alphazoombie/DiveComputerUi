#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include <iostream>
#include <SD.h>
#include <string>
#include <fstream>
#include "WifiAccessGenerator.h"
#include "Helper.h"

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
    int m_diveID = 1;
    char m_current_date[9];
    char m_oldDate[9];
    char m_logPath[25] PROGMEM;
    bool m_sameSession = false;
    void setDateToFile(); 
    void getDateFromFile();
    int  getDiveID(); 
    void setDiveID();
    void createWifiDataFile();
    void writeDateToSessionFile(char* date); 
    void createLastDiveFile(char* path);
    void getCurrentDate(); 
    void initializeMetaData();
    void createDirectory();  
    bool fileExists();
};

#endif