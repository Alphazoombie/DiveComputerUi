#include "PeripheralManager.h"

int PeripheralManager::m_resPin = 6;
int PeripheralManager::m_mfioPin = 5;
int PeripheralManager::m_chipSelect = 14;

SparkFun_Bio_Sensor_Hub PeripheralManager::m_bioHub = SparkFun_Bio_Sensor_Hub(m_resPin, m_mfioPin);
bioData PeripheralManager::m_body;

//TODO: use other library for ESP32 the SparkFun library didn´t work on the ESP32
//-------biohub sensor--------
//initializes the biohub-sensor
void PeripheralManager::initializeBioHubSensor() 
{
    int resPin = 6;
    int mfioPin = 5;

    m_bioHub = SparkFun_Bio_Sensor_Hub(resPin, mfioPin);
    int result = m_bioHub.begin();
    if (result == 0) // Zero errors!
      Serial.println("Sensor started!");
    else
      Serial.println("Could not communicate with the sensor!!!");
    
    Serial.println("Configuring Sensor...."); 
    int error = m_bioHub.configBpm(MODE_ONE); // Configuring just the BPM settings. 
    if(error == 0) // Zero errors!
    { 
      Serial.println("Sensor configured");
    }
    else 
    {
      Serial.println("Error configuring sensor.");
      Serial.print("Error: "); 
      Serial.println(error); 
    }    
}

//-------SD card--------
//initializes the SD card
void PeripheralManager::initializeSD() 
{
    Serial.print("Initializing SD card...");
    // see if the card is present and can be initialized:
    SD.begin(m_chipSelect);
    if (!SD.begin(m_chipSelect)) 
    {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      while (1);
    }
    Serial.println(" card initialized.");
}

//TODO: get real-time clock value 
//get time from real-time clock
void PeripheralManager::getCurrentTime(char* time)
{
    strcpy(time, "14:50:13");
}

void PeripheralManager::getCurrentDate(char* date)
{
    //TBI
}
