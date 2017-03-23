#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include "../environment/Tile.h"

class GraphicTilemap;
class GraphicTile {
    GraphicTilemap &tilemap;


    void setupVerticles(unsigned tId, unsigned tileWidth, unsigned tileHeight, unsigned firstgid);


    Tile &dataTile;


public:
    GraphicTile(Tile &dataTile, GraphicTilemap &pTilemap);
    sf::Vertex vertices[4];

    // Getters
    sf::Vector2f getPixelPosition()const;

};




