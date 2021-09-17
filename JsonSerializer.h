#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <ArduinoJson.h>
#include "FileSystem.h"
#include "DiveData.h"

class FileSystem;

class JsonSerializer
{
public:
	static void createJsonString(DiveData* diveData, FileSystem* fileSystem, char* jsonResultString);
};

#endif