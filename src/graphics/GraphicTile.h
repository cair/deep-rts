#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include "../environment/Tile.h"

class GraphicTilemap;
class GraphicTile {
    GraphicTilemap &tilemap;


    void setupVerticles(sf::Vertex* vertices, int tId, int tileWidth, int tileHeight, int firstgid);




public:
    Tile &dataTile;
    GraphicTile(Tile &dataTile, GraphicTilemap &pTilemap);
    sf::Vertex tile_verticles[4];
    sf::Vertex deplete_verticles[4];

    // Getters
    sf::Vector2f getPixelPosition()const;

};




