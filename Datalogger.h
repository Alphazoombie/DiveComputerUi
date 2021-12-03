/**
 * @author Manuel Bäcker
 * @author Jan Tröger
 * @author Tim Tröger
 */

#ifndef DATALOGGER_H
#define DATALOGGER_H

#include "JsonSerializer.h"
#include "PeripheralManager.h"
#include "FileSystem.h"
#include "DiveData.h"
#include "UISystem.h"

class Datalogger 
{
    public:
        void getData(FileSystem* fileSystem);
        void logData(FileSystem* fileSystem, char* fullFilePath);
    
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