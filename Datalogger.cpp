#include "Datalogger.h"

// Reads data from the sensors and calls the method "createJsonString" with the data as 
// parameter.
void Datalogger::getData(FileSystem* fileSystem) 
{   
    DiveData::diveData->accelX = 1.1f;
    DiveData::diveData->accelY = 1.2f;
    DiveData::diveData->accelZ = 1.3f;
    DiveData::diveData->depth = 5.5f;
    DiveData::diveData->time = 300000;
    DiveData::diveData->gyroX = 2.1f;
    DiveData::diveData->gyroY = 2.2f;
    DiveData::diveData->gyroZ = 2.3f;
    DiveData::diveData->heartFrequency = 75;
    DiveData::diveData->heartVariability = 100;
    DiveData::diveData->brightness = 50;
    DiveData::diveData->o2saturation = 98;
    DiveData::diveData->temperatur = 18.5f;
}



// This function saves a String (Json-string with our data) in a file on the SD card.
void Datalogger::logData(FileSystem* fileSystem, char* fullFilePath) 
{
    char data[200];
    JsonSerializer::createJsonString(DiveData::diveData, fileSystem, data);
    m_dataFile = SD.open(fullFilePath, FILE_APPEND); //logPath
    if (m_dataFile)
    {
        m_dataFile.println(data);
        m_dataFile.close();
        Serial.println("data logged...");
        delay(50);
    }
    else 
    {
        Serial.println("error opening logfile");
    }
}

// void Datalogger::initialize(PeripheralManager* peripheralManager)
// {
//     m_peripheralManager = peripheralManager;
// }
