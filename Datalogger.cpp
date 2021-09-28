#include "Datalogger.h"

// Reads data from the sensors and calls the method "createJsonString" with the data as 
// parameter.
void Datalogger::getData(FileSystem* fileSystem) 
{   
    //Currently hardcoded dummy data
    UISystem::currentDiveData.accelX = 1.1f;
    UISystem::currentDiveData.accelY = 1.2f;
    UISystem::currentDiveData.accelZ = 1.3f;
    UISystem::currentDiveData.depth = random(3, 10)*1.0f;
    if (millis() > 1000)
    {
        UISystem::currentDiveData.time = millis() / 1000;
    }
    else
    {
        UISystem::currentDiveData.time = 1;
    }    
    UISystem::currentDiveData.gyroX = 2.1f;
    UISystem::currentDiveData.gyroY = 2.2f;
    UISystem::currentDiveData.gyroZ = 2.3f;
    UISystem::currentDiveData.heartFrequency = 75;
    UISystem::currentDiveData.heartVariability = 100;
    UISystem::currentDiveData.brightness = 50;
    UISystem::currentDiveData.o2saturation = 98;
    UISystem::currentDiveData.temperatur = 18.5f;
}

// This function saves a String (Json-string with our data) in a file on the SD card.
void Datalogger::logData(FileSystem* fileSystem, char* fullFilePath) 
{
    char data[200];
    JsonSerializer::createJsonString(fileSystem, data);
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
