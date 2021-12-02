#include "FileSystem.h"

// int FileSystem::m_diveID = 1;

//writes the value of variable date in "date.log"
void FileSystem::setDateToFile() 
{
    File file = SD.open(m_datePath, FILE_WRITE);
    if(file) 
    {
        file.print(m_current_date);
        file.close();
    }
}

// reades the date from "date.log" and writes it
// in the m_oldDate member variable
void FileSystem::getDateFromFile() 
{
    File file = SD.open(m_datePath);
    
    if(file) 
    {
        file.readBytesUntil('\0', m_oldDate, 9);
        m_oldDate[8] = '\0';

        Serial.print("old date read from file: ");
        Serial.println(m_oldDate);	
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
        int diveID = file.parseInt();
        file.close();
        return diveID;
    }
    return -1;
}

// writes the value of variable m_diveID into "dive.log"
void FileSystem::setDiveID() 
{
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
    File file = SD.open(m_sessionsPath, FILE_APPEND);    
    if(file) 
    {
        file.println(date);
        file.close();
    }
}

// has to be implemented with the real-time-clock when built in;
// get date from the real-time-clock and write value into date-variable
void FileSystem::getCurrentDate() 
{	
    //char out[20];
    //snprintf(out, sizeof out, "%id", millis());
    //strncpy ( date, out, 6);
    
    // char currDate[] = "05_06_21";
    strcpy(m_current_date, "21_10_21");
}

// -------META files--------
// sets the date; checks if the meta-files "dive.log" and "date.log"
// exist and creates them if not; if yes the variables "diveID" and
// "date" are set; the current date is compared to the date from
// "date.log" to check if its a new session; if its not a new session 
// the bool "sameSession" is set to true; 
void FileSystem::initializeMetaData() 
{
    getCurrentDate();
    Serial.print("olddate sollte leer sein -> ");
    Serial.println(m_oldDate);
    if(!SD.exists(m_directoryPath))
    {
        SD.mkdir(m_directoryPath);
    }
    
    if (SD.exists(m_datePath)) 
    {
        getDateFromFile();
        Serial.print("new date -> ");
        Serial.println(m_current_date);
        Serial.print("old date -> ");
        Serial.println(m_oldDate);
        if (strcmp(m_current_date, m_oldDate) != 0)
        {
            SD.remove(m_datePath);
            setDateToFile();
        } 
        else 
        {
            char datePath[20] = "/logFiles/";
            Helper::concatCharArrays(datePath, m_current_date);

            if (SD.exists(datePath))
            {
                m_sameSession = true;
            }
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
    }
    setDiveID();  
}

void FileSystem::createLastDiveFile(char* path)
{
    File file = SD.open("/lastDive.log", FILE_WRITE);
    if (file)
    {
        file.println(path);
        file.close();
    }  
}

//creates a directory with given member variable m_directoryPath
void FileSystem::createDirectory() 
{
    SD.mkdir(m_directoryPath);
}

//Creates the file where the credentials of the WIFI AP are stored
//SSIDs are stored in the form of DC_XXXXX where the 5 digits are randomly generated
//Passwords are 8 digits long and consist of random numbers
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
