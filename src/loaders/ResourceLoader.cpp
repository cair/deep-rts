//
// Created by per-arne on 02.04.17.
//

#include <fstream>
#include <iostream>
#include "ResourceLoader.h"
#include "../../../include/rapidjson/include/rapidjson/istreamwrapper.h"
#include <cstdio>
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

std::string ResourceLoader::getFilePath(const std::string& fileName) {
    std::string workingDir = GetCurrentWorkingDir();
    std::string assetLocation = "DeepRTS/python";

    if (workingDir.find(assetLocation) != std::string::npos) {
        // asset location is included (happens in python runtime)
        return workingDir + "/assets/" + fileName;

    } else {
        // Did not find asset location in Working directory
        return workingDir + "/" + assetLocation + "/assets/" + fileName;
    }

}


void ResourceLoader::loadMapJSON(const std::string& mapFile) {

    // If map is already loaded
    if(mapLoaded) {
        return;
    }

    // Retrieve map file location
	auto mapFilePath = getFilePath("maps/" + mapFile);

    // Read Map data
    std::ifstream map(mapFilePath);

	if (map.is_open()) {
		rapidjson::IStreamWrapper isw(map);
		rapidjson::Document d;
		mapJSON.ParseStream(isw);
		mapLoaded = true;
		if(mapJSON.IsNull()){
			throw std::runtime_error("File Error: Could not find: " + mapFile + " (" + mapFilePath + ").\nEnsure that the data directory exists!");
		}
	}
	else {
		throw std::runtime_error("File Error: Could not find: " + mapFile + " (" + mapFilePath + ").\nEnsure that the data directory exists!");
	}

}

void ResourceLoader::loadTileJSON() {


	std::string fileName = "tile_properties.json";
	std::string filePath = getFilePath(fileName);

    std::ifstream map(filePath);
	if (map.is_open()) {
		rapidjson::IStreamWrapper isw(map);
		tileJSON.ParseStream(isw);
		return;
	}
	throw std::runtime_error("File Error: Could not find: " + fileName + "(" + filePath + ").\nEnsure that the data directory exists!");


}

