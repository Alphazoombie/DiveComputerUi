#include "FileSystem.h"

int FileSystem::m_diveID = 1;

//writes the value of variable date in "date.log"
void FileSystem::setDateToFile() 
{
    File file = SD.open(m_datePath, FILE_WRITE);
    if(file) 
    {
        file.print(m_date.c_str());
        file.close();
    }
}
// reades the date from "date.log" and writes it
// in the character-array which is referenced in
// the parameter
void FileSystem::getDateFromFile() // char* result
{
    File file = SD.open(m_datePath);
    
    if(file) 
    {
        // strcpy(result, "16_04_21");
        // Serial.print("result bevor readString -> ");
        // Serial.println(result);
        Serial.print("direkt vom file gelesen -> ");
        String x = file.readString();
        Serial.println(x);
        
        //file.readString().toCharArray(result, dateLength);
        m_oldDate = file.readString().c_str();
        Serial.print("result aka altes date -> ");
        Serial.println(m_oldDate.c_str());	
        file.close();
    }
}
// returns the diveID from "dive.log"
// returns -1 if failed to open the file
int FileSystem::getDiveID() 
{
    File file = SD.open(m_divePath);
    if(file) 
    {
        int x = file.parseInt();
        file.close();
        return x;
    }
    return -1;
}
// writes the value of variable diveID in "dive.log"
void FileSystem::setDiveID() 
{
    if(SD.exists(m_divePath))
    {
        SD.remove(m_divePath);
    }
    File file = SD.open(m_divePath, FILE_WRITE);
    if(file) 
    {
        file.print(m_diveID);
        file.close();
    }
}
// writes a date in "sessions.log"
void FileSystem::writeDateToSessionFile(char* date) 
{
    File file = SD.open(m_sessionsPath, FILE_WRITE);    
    if(file) 
    {
        file.println(date);
        file.close();
    }
}
// has to be implemented with the real-time-clock when built in;
// get date from the real-time-clock and write value into date-variable
void FileSystem::setDate() 
{	
    //char out[20];
    //snprintf(out, sizeof out, "%id", millis());
    //strncpy ( date, out, 6);
    
    // char currDate[] = "05_06_21";
    // strcpy(m_date, currDate);
    m_date = "12_11_21";
}
// -------META files--------
// sets the date; checks if the meta-files "dive.log" and "date.log"
// exist and creates them if not; if yes the variables "diveID" and
// "date" are set; the current date is compared to the date from
// "date.log" to check if its a new session; if its not a new session 
// the bool "sameSession" is set to true; 
void FileSystem::initializeMetaData() 
{
    setDate();
    Serial.print("olddate sollte leer sein -> ");
    Serial.println(m_oldDate.c_str());
    if(!SD.exists(m_directoryPath))
    {
        SD.mkdir(m_directoryPath);
    }
    
    if (SD.exists(m_datePath)) 
    {
        // getDateFromFile(m_oldDate.c_str());
        // strcpy(m_date, "12_11_21");
        // getDateFromFile(m_oldDate.c_str());
        // strcpy(m_date, "36_11_21");
        getDateFromFile();
        Serial.print("new date -> ");
        Serial.println(m_date.c_str());
        Serial.print("old date -> ");
        Serial.println(m_oldDate.c_str());
        if (m_date != m_oldDate) //strcmp(m_date, m_oldDate) != 0
        {
            SD.remove(m_datePath);
            setDateToFile();
        } 
        else 
        {
            m_sameSession = true;
        }
    } 
    else 
    {
        setDateToFile();
    }
    
    if(SD.exists(m_divePath)) 
    {
        if(m_sameSession) 
        {
            m_diveID = getDiveID();
        }
        //SD.remove(divePath);
    }
    setDiveID();  
}
//creates a directory with given parameter as name
void FileSystem::createDirectory() 
{
    SD.mkdir(m_directoryPath);
}

void FileSystem::createWifiDataFile()
{
    if (!SD.exists(m_wifiDataPath))
    {
        char ssid[9] = "DC_";
        char password[9];

        File file = SD.open(m_wifiDataPath, FILE_APPEND);
        Serial.println("Creating wifi data file...");
        if (file)
        {
            WifiAccessGenerator::generateSsid(ssid);
            WifiAccessGenerator::generatePassword(password);
            file.println(ssid);
            file.println(password);
            file.close();
            Serial.println("Wifi Access data successfully written to file.");
        }
    }
}
