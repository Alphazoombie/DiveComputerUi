#ifndef PERIPHERALMANAGER_H
#define PERIPHERALMANAGER_H

#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <SD.h>
#include <SPI.h>

#define LOOP_STATE_STOPPED 0
#define LOOP_STATE_STARTED 1

class PeripheralManager
{
public:
    static int m_resPin;
    static int m_mfioPin;
    static int m_chipSelect;

    static SparkFun_Bio_Sensor_Hub m_bioHub;
    static bioData m_body;
    
    static void initializeBioHubSensor();
    static void initializeSD();
    static void getCurrentTime(char* time);
    static void getCurrentDate(char* date);
};

#endif