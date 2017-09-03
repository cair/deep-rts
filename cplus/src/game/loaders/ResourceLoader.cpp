//
// Created by per-arne on 02.04.17.
//

#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <iostream>
#include "ResourceLoader.h"


void ResourceLoader::loadMapJSON(std::string map_file) {
    if(mapLoaded) return;
    mapLoaded = true;
    // Read Map data
    std::ifstream map(".//data//maps//" + map_file);
    if(map){
        rapidjson::IStreamWrapper isw(map);
        rapidjson::Document d;
        mapJSON.ParseStream(isw);
    } else {
        std::cout << "Could not find \"data/maps/" << map_file << "\"" << std::endl;
        exit(0);
    }

}

void ResourceLoader::loadTileJSON() {
    std::ifstream map(".//data//tile_properties.json");
    rapidjson::IStreamWrapper isw(map);
    tileJSON.ParseStream(isw);
}

void ResourceLoader::loadConfigJSON() {
        std::ifstream confData(".//data//config.json");
        if(confData){
            rapidjson::IStreamWrapper isw(confData);
            configJSON.ParseStream(isw);
        } else {
            std::cout << "Could not find \"data/config.json\"" << std::endl;
            exit(0);
        }

}