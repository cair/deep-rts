//
// Created by Per-Arne on 23.02.2017.
//

#include <fstream>
#include <set>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "MapLoader.h"

using json = nlohmann::json;



Tilemap MapLoader::load(std::string map_name) {
    json mapData = MapLoader::loadFile(map_name);
    json mapLayer = mapData["layers"][0];
    json tileIDs = mapLayer["data"];

    json tilesData = MapLoader::tileProperties();
    sf::Texture tilesetTexture = MapLoader::loadTexture();
    Tilemap tilemap = Tilemap(tilesData, mapData, tilesetTexture);

    return tilemap;
}

json MapLoader::loadFile(std::string map_file) {
    // Read Map data
    std::ifstream map("./data/maps/" + map_file);
    json data;
    map >> data;
    return data;
}

json MapLoader::tileProperties() {
    std::ifstream map("./data/tile_properties.json");
    json data;
    map >> data;
    return data;
}

sf::Texture MapLoader::loadTexture() {
    // Load textures
    sf::Texture texture;
    texture.loadFromFile("data/textures/tiles.png");
    return texture;
}


