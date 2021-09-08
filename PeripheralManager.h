#pragma once
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

    //NEEDED?
    //static int loopStateDive = LOOP_STATE_STOPPED;
    //static int loopStateBluetooth = LOOP_STATE_STOPPED;

    SparkFun_Bio_Sensor_Hub m_bioHub = SparkFun_Bio_Sensor_Hub(m_resPin, m_mfioPin);
    bioData m_body;
    
    void initializeBioHubSensor();
    void initializeSD();
};











