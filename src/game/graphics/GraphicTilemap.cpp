//
// Created by per on 3/16/17.
//

#include "GraphicTilemap.h"
#include <iostream>
#include <fstream>


GraphicTilemap::GraphicTilemap(std::vector<Tile> &dataTiles) {
    loadTexture();
    generateGraphicTiles(dataTiles);

}

void GraphicTilemap::generateGraphicTiles(std::vector<Tile> &dataTiles) {

    // Iterate through all dataTiles and create a equivalent graphic tile
    for(auto &dataTile : dataTiles){

        gTiles.push_back(GraphicTile(dataTile, *this));


    }


}

void GraphicTilemap::loadTexture() {
    // Load textures
	if (!tileset.loadFromFile("data//textures//tiles.png")) {
		std::cout << "Could not load .\\data\\textures\\tiles.png" << std::endl;
		throw;
	}
}
