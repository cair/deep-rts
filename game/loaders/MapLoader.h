//
// Created by Per-Arne on 23.02.2017.
//

#ifndef WARC2SIM_MAPLOADER_H
#define WARC2SIM_MAPLOADER_H

#include <string>

#include "../environment/Tile.h"
#include <SFML/Graphics/Texture.hpp>
#include "../lib/json.h"
#include "../environment/Tilemap.h"

using json = nlohmann::json;


class MapLoader {

    static json loadFile(std::string map_file);
    static Tilemap getTileset(json tilesData, json basic_json);

public:

    static Tilemap load(std::string map_name) ;
    static Tilemap loadNull();

    static json tileProperties();

    static sf::Texture loadTexture();
};


#endif //WARC2SIM_MAPLOADER_H
