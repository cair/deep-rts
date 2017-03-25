//
// Created by Per-Arne on 23.02.2017.
//

#ifndef WARC2SIM_MAPLOADER_H
#define WARC2SIM_MAPLOADER_H

#include <string>

#include "../environment/Tile.h"
#include "../third_party/json.hpp"
#include "../environment/Tilemap.h"

using json = nlohmann::json;


class MapLoader {

public:
	static json loadFile(std::string mapFile);
    static Tilemap getTileset(json tilesData, json basic_json);
    static json tileProperties();

};


#endif //WARC2SIM_MAPLOADER_H
