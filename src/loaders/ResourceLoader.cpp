//
// Created by per-arne on 02.04.17.
//

#include <fstream>
#include <iostream>
#include "ResourceLoader.h"
#include "../../include/rapidjson/include/rapidjson/istreamwrapper.h"
#include <stdio.h>


#include <stdio.h>
#include <string>
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#elif __linux__
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

std::string GetCurrentWorkingDir()
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}

void ResourceLoader::loadMapJSON(std::string map_file) {


    if(mapLoaded) return;
	std::string filePath = GetCurrentWorkingDir() + "/data/maps/" + map_file;

    // Read Map data
    std::ifstream map(filePath);

	if (map.is_open()) {
		rapidjson::IStreamWrapper isw(map);
		rapidjson::Document d;
		mapJSON.ParseStream(isw);
		mapLoaded = true;
	}
	else {
		throw std::runtime_error("File Error: Could not find: " + map_file + " (" + filePath + ").\nEnsure that the data directory exists!");
	}

}

void ResourceLoader::loadTileJSON() {
	std::string fileName = "tile_properties.json";
	std::string filePath = GetCurrentWorkingDir() + "/data/" + fileName;

    std::ifstream map(filePath);
	if (map.is_open()) {
		rapidjson::IStreamWrapper isw(map);
		tileJSON.ParseStream(isw);
		return;
	}
	throw std::runtime_error("File Error: Could not find: " + fileName + "(" + filePath + ").\nEnsure that the data directory exists!");


}

void ResourceLoader::loadConfigJSON() {
	std::string fileName = "config.json";
	std::string backupFilePath = GetCurrentWorkingDir() + "/data/" + fileName;
	std::string filePath = "./" + fileName;

	// Attempt to read config file in current working directory (from python for example)
	std::ifstream confData(filePath);
	if (confData.is_open()) {
		rapidjson::IStreamWrapper isw(confData);
		configJSON.ParseStream(isw);
		return;
	}

	// Attempt to read the default config file
	std::ifstream confData2(backupFilePath);
	if (confData2.is_open()) {
		rapidjson::IStreamWrapper isw(confData2);
		configJSON.ParseStream(isw);
		return;
	}

	throw std::runtime_error("File Error: Could not find: " + fileName + " (" + filePath + ", or " + backupFilePath + ").\nEnsure that the data directory exists!");
}