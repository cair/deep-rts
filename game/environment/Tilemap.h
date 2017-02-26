//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_TILESET_H
#define WARC2SIM_TILESET_H


#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Tile.h"
#include "../lib/json.h"
using json = nlohmann::json;

class Tilemap: public sf::Drawable{
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


public:
    sf::Texture tileset;
    std::vector<Tile> tiles;
    std::vector<int> spawnTiles;

    int TILE_WIDTH;
    int TILE_HEIGHT;
    int MAP_WIDTH;
    int MAP_HEIGHT;
    float tileTextureDimension;
    float tileWorldDimension;
    Tilemap(json tilesData, json mapData, sf::Texture tileset);

    void addTileVertices(unsigned tId, unsigned tileWidth, unsigned tileHeight, unsigned firstgid, Tile &tile);

    Tilemap();
};


#endif //WARC2SIM_TILESET_H
