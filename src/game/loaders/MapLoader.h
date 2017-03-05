//
// Created by Per-Arne on 23.02.2017.
//

#ifndef WARC2SIM_MAPLOADER_H
#define WARC2SIM_MAPLOADER_H

#include <string>

#include "../environment/Tile.h"
#include <SFML/Graphics/Texture.hpp>
#include "../third_party/json.hpp"

#include "../environment/Tilemap.h"

using json = nlohmann::json;


class MapLoader {

    


public:
	static json loadFile(std::string mapFile);
    static Tilemap getTileset(json tilesData, json basic_json);
    static json tileProperties();
    static sf::Texture loadTexture();
};


#endif //WARC2SIM_MAPLOADER_H
