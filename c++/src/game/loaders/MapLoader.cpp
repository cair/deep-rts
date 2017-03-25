//
// Created by Per-Arne on 23.02.2017.
//

#include <fstream>
#include "MapLoader.h"

using json = nlohmann::json;

json MapLoader::loadFile(std::string map_file) {
    // Read Map data
    std::ifstream map(".//data//maps//" + map_file);
    json data;
    map >> data;
    return data;
}

json MapLoader::tileProperties() {
    std::ifstream map(".//data//tile_properties.json");
    json data;
    map >> data;
    return data;
}

