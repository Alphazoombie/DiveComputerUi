#ifndef DATALOGGER_H
#define DATALOGGER_H

#include "JsonSerializer.h"
#include "PeripheralManager.h"
#include "FileSystem.h"

class Datalogger 
{
    public:
        // void initialize(PeripheralManager* peripheralManager);
        void getData(char* jsonString);
        void logData(char* fullFilePath, char* data);

        void test();
    
    private:
        float m_accX;
        float m_accY;
        float m_accZ;
        float m_gyrX;
        float m_gyrY;
        float m_gyrZ;
        File m_dataFile;
        PeripheralManager* m_peripheralManager;
};

#endif