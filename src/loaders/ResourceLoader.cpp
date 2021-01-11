//
// Created by per-arne on 02.04.17.
//

#include <fstream>
#include <iostream>
#include "ResourceLoader.h"
#include "../../../include/rapidjson/include/rapidjson/istreamwrapper.h"
#include "../util/String.h"
#include <cstdio>
#include <string>
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#elif defined __linux__ || defined __APPLE__
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
	    try{
            mapJSON = nlohmann::json::parse(map);
	    }catch(const nlohmann::json::exception&){
            throw std::runtime_error("Failed to parse: " + mapFile + " (" + mapFilePath + ").");
	    }
		mapLoaded = true;
	}
	else {
		throw std::runtime_error("File Error: Could not find: " + mapFile + " (" + mapFilePath + ").\nEnsure that the data directory exists!");
	}

}

TilePropertyData ResourceLoader::loadMAPJson(const nlohmann::json& tJSON){

    TilePropertyData data;
    // Parse tile types
    for(auto &item : tJSON["tile_types"].items()){

        for(auto &ele : item.value()){

            if(ele.is_string()){
                auto range = StringUtil::split(ele.get<std::string>(), "-");
                for(auto i = std::stoi(range[0]); i <= std::stoi(range[1]); i++){
                    data.tileID2Type.try_emplace(i, item.key());
                }
            }else if(ele.is_number_integer()){

                data.tileID2Type.try_emplace(ele.get<int>(), item.key());

            }

        }
    }

    // Parse tile data
    for(auto &item : tJSON["tile_metadata"].items()){
        //std::cout << item.key() << std::endl;
        data.tileData.try_emplace(item.key(), item.value());
    }
    return data;
}

void ResourceLoader::loadTileJSON() {

	std::string fileName = "tile_properties.json";
	std::string filePath = getFilePath(fileName);

    std::ifstream map(filePath);
	if (map.is_open()) {
	    try{
            tileJSON = nlohmann::json::parse(map);
        }catch(const nlohmann::json::exception&){
            throw std::runtime_error("Failed to parse: " + fileName + " (" + filePath + ").");
	    }
	    tileData = ResourceLoader::loadMAPJson(tileJSON);

        tilesLoaded = true;
	}else{
        throw std::runtime_error("File Error: Could not find: " + fileName + "(" + filePath + ").\nEnsure that the data directory exists!");
	}

}

