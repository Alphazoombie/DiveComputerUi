#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <ArduinoJson.h>
//#include "FileSystem.h"

class FileSystem;

class JsonSerializer
{
public:
	static void createJsonString(float accX, 
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
								 char* jsonResultString);

};

#endif