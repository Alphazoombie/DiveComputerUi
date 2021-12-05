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
    PeripheralManager(){}

    int m_resPin = 6;
    int m_mfioPin = 5;
    int m_chipSelect = 14;

    SparkFun_Bio_Sensor_Hub m_bioHub = SparkFun_Bio_Sensor_Hub(m_resPin, m_mfioPin);
    bioData m_body;
    
    void initializeBioHubSensor();
    void initializeSD();
    static void getCurrentTime(char* time);
};

#endif