#include "JsonSerializer.h"

//returns a Json String with values: diveID, time, heartrate, oxygen, confidence, depth, brightness, temperature, accX, accY, accZ, gyrX, gyrY, gyrZ
void JsonSerializer::createJsonString(FileSystem* fileSystem, char* jsonResultString)
{
	DynamicJsonDocument jsonObject(256);
	jsonObject["1"]  = UISystem::currentDiveData.accelX;
	jsonObject["2"]  = UISystem::currentDiveData.accelY;
	jsonObject["3"]  = UISystem::currentDiveData.accelZ;
	jsonObject["4"]  = UISystem::currentDiveData.depth;
	jsonObject["5"]  = UISystem::currentDiveData.time;
	jsonObject["6"]  = UISystem::currentDiveData.gyroX; 
	jsonObject["7"]  = UISystem::currentDiveData.gyroY;
	jsonObject["8"]  = UISystem::currentDiveData.gyroZ; 
	jsonObject["9"]  = UISystem::currentDiveData.heartFrequency;
	jsonObject["10"] = UISystem::currentDiveData.heartVariability;
	jsonObject["11"] = UISystem::currentDiveData.brightness;
	jsonObject["12"] = UISystem::currentDiveData.o2saturation;
	jsonObject["13"] = fileSystem->m_diveID;
	jsonObject["14"] = UISystem::currentDiveData.temperatur;
	
	serializeJson(jsonObject, jsonResultString, 200);
}
