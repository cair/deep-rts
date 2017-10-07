//
// Created by per-arne on 02.04.17.
//

#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <iostream>
#include "ResourceLoader.h"


void ResourceLoader::loadMapJSON(std::string map_file) {
    if(mapLoaded) return;
	std::string filePath = ".//data//maps//" + map_file;

    // Read Map data
    std::ifstream map(filePath);

	if (map.is_open()) {
		rapidjson::IStreamWrapper isw(map);
		rapidjson::Document d;
		mapJSON.ParseStream(isw);
		mapLoaded = true;
	}
	else {
		std::cout << "Could not read " << filePath.c_str() << ". Ensure that path exists!" << std::endl;
		throw "File Error!";
	}

}

void ResourceLoader::loadTileJSON() {
    std::ifstream map(".//data//tile_properties.json");
	if (map.is_open()) {
		rapidjson::IStreamWrapper isw(map);
		tileJSON.ParseStream(isw);
		return;
	}

	throw "Could not read .//data//tile_properties.json";

}

void ResourceLoader::loadConfigJSON() {
    std::ifstream confData(".//data//config.json");
	if (confData.is_open()) {
		rapidjson::IStreamWrapper isw(confData);
		configJSON.ParseStream(isw);
		return;
	}

	throw "Could not read .//data//config.json";
}