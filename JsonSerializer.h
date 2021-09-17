#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <ArduinoJson.h>
#include "FileSystem.h"
#include "DiveData.h"
#include "UISystem.h"

class FileSystem;

class JsonSerializer
{
public:
	static void createJsonString(FileSystem* fileSystem, char* jsonResultString);
};

#endif