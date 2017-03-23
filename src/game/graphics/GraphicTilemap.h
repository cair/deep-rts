//
// Created by per on 3/16/17.
//

#ifndef DEEPRTS_GRAPHICTILEMAP_H
#define DEEPRTS_GRAPHICTILEMAP_H

#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include "GraphicTile.h"

class Tile;
class GraphicTilemap {


    void loadTexture();
    void generateGraphicTiles(std::vector<Tile> &vector);
public:
    GraphicTilemap(std::vector<Tile>& dataTiles);
    std::vector<GraphicTile> gTiles;
    sf::Texture tileset;

};



#endif //DEEPRTS_GRAPHICTILEMAP_H
