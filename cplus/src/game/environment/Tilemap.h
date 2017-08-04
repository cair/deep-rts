//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_TILESET_H
#define WARC2SIM_TILESET_H



#include "../Constants.h"
#include "Tile.h"
#include <vector>
#include "rapidjson/document.h"

class Game;
class Tilemap{
private:


public:
    std::vector<Tile> tiles;
    std::vector<int> spawnTiles;
	Game *game_;

    size_t TILE_WIDTH;
    size_t TILE_HEIGHT;
    size_t MAP_WIDTH;
    size_t MAP_HEIGHT;
    float tileTextureDimension;
    float tileWorldDimension;
    Tilemap(std::string mapName, Game *game);

	std::vector<Tile>& getTiles();
    std::vector<Tile *> neighbors(Tile &tile, Constants::Pathfinding type);
    std::vector<Tile *> getTiles(Tile *source, int width, int height);

    Tile *getTile(int x, int y);

    rapidjson::Document tilesData;
    int tFirstGid;

	bool operator()(unsigned x, unsigned y) const;
};


#endif //WARC2SIM_TILESET_H
