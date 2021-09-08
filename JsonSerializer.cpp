#include "JsonSerializer.h"
#include "FileSystem.h"

//returns a Json String with values: diveID, time, heartrate, oxygen, confidence, depth, brightness, temperature, accX, accY, accZ, gyrX, gyrY, gyrZ
void JsonSerializer::createJsonString(float accX, 
                                      float accY, 
                                      float accZ, 
                                      float depth, 
                                      long duration, 
                                      float gyrX, 
                                      float gyrY, 
                                      float gyrZ, 
                                      int heartrate, 
                                      int confidence,
 						              int brightness, 
                                      int oxygen, 
                                      int diveID, 
                                      float waterTemperature, 
                                      char* jsonResultString)
{
	DynamicJsonDocument jsonObject(256);
	jsonObject["1"] = accX;
	jsonObject["2"] = accY;
	jsonObject["3"] = accZ;
	jsonObject["4"] = depth;
	jsonObject["5"] = duration;
	jsonObject["6"] = gyrX;
	jsonObject["7"] = gyrY;
	jsonObject["8"] = gyrZ;
	jsonObject["9"] = heartrate;
	jsonObject["10"] = confidence;
	jsonObject["11"] = brightness;
	jsonObject["12"] = oxygen;
	jsonObject["13"] = FileSystem::m_diveID;
	jsonObject["14"] = waterTemperature;
	
	serializeJson(jsonObject, jsonResultString, 200);
}