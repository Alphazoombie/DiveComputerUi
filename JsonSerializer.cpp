#include "JsonSerializer.h"

//returns a Json String with values: diveID, time, heartrate, oxygen, confidence, depth, brightness, temperature, accX, accY, accZ, gyrX, gyrY, gyrZ
void JsonSerializer::createJsonString(DiveData* diveData, FileSystem* fileSystem, char* jsonResultString)
{
	DynamicJsonDocument jsonObject(256);
	jsonObject["1"] = diveData->accelX;
	jsonObject["2"] = diveData->accelY;
	jsonObject["3"] = diveData->accelZ;
	jsonObject["4"] = diveData->depth;
	jsonObject["5"] = diveData->time;
	jsonObject["6"] = diveData->gyroX; 
	jsonObject["7"] = diveData->gyroY;
	jsonObject["8"] = diveData->gyroZ; 
	jsonObject["9"] = diveData->heartFrequency;
	jsonObject["10"] = diveData->heartVariability;
	jsonObject["11"] = diveData->brightness;
	jsonObject["12"] = diveData->o2saturation;
	jsonObject["13"] = fileSystem->m_diveID;
	jsonObject["14"] = diveData->temperatur;
	
	serializeJson(jsonObject, jsonResultString, 200);
}
