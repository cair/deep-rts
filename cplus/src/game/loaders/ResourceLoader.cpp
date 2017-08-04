//
// Created by per-arne on 02.04.17.
//

#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include "ResourceLoader.h"


void ResourceLoader::loadMapJSON(std::string map_file) {
    if(mapLoaded) return;

    mapLoaded = true;
    // Read Map data
    std::ifstream map(".//data//maps//" + map_file);
    rapidjson::IStreamWrapper isw(map);
    rapidjson::Document d;
    mapJSON.ParseStream(isw);
}

void ResourceLoader::loadTileJSON() {
    std::ifstream map(".//data//tile_properties.json");
    rapidjson::IStreamWrapper isw(map);
    tileJSON.ParseStream(isw);
}

void ResourceLoader::loadConfigJSON() {
    std::ifstream confData(".//data//config.json");
    rapidjson::IStreamWrapper isw(confData);
    configJSON.ParseStream(isw);
}