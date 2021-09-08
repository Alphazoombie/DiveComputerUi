#include "Datalogger.h"

// Reads data from the sensors and calls the method "createJsonString" with the data as 
// parameter.
void Datalogger::getData(char* jsonString) 
{   
    JsonSerializer jsonSerializer;
    jsonSerializer.createJsonString(
                                    m_accX, 
                                    m_accY, 
                                    m_accZ,
                                    (float)random(0, 50),
                                    millis(),
                                    m_gyrX, 
                                    m_gyrY, 
                                    m_gyrZ,
                                    m_peripheralManager->m_body.heartRate, 
                                    m_peripheralManager->m_body.confidence, 
                                    random(0,255), 
                                    m_peripheralManager->m_body.oxygen, 
                                    FileSystem::m_diveID,
                                    (float)random(0,30),                     
                                    jsonString);

    // jsonSerializer.createJsonString(
    //     1.1,
    //     1.2,
    //     1.3,
    //     5.5,
    //     300000,
    //     2.1,
    //     2.2,
    //     2.3,
    //     75,
    //     100,
    //     50,
    //     98,
    //     18.5,
    //     jsonString
    // );
    Serial.println("got the data bro");
}                         

// This function saves a String (Json-string with our data) in a file on the SD card.
void Datalogger::logData(char* fullFilePath, char* data) 
{
    m_dataFile = SD.open(fullFilePath, FILE_WRITE); //logPath
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