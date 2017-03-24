//
// Created by Per-Arne on 24.02.2017.
//
#include "Tilemap.h"
#include "./Tile.h"
#include "../unit/Unit.h"
#include "../Game.h"

Tilemap::Tilemap(std::string mapName, Game *game): game_(game){
	json mapData = MapLoader::loadFile(mapName);
	json tilesData = MapLoader::tileProperties();



    json tilesetData = mapData["tilesets"][0];
    json mapLayer = mapData["layers"][0];
    json tileIDs = mapLayer["data"];

    uint16_t mapWidth = mapData["width"];
	uint16_t mapHeight = mapData["height"];
	uint16_t tWidth = tilesetData["tilewidth"];
	uint16_t tHeight = tilesetData["tileheight"];
    tFirstGid = tilesetData["firstgid"];

    TILE_WIDTH = tWidth;
    TILE_HEIGHT = tHeight;
    MAP_WIDTH = mapWidth;
    MAP_HEIGHT = mapHeight;
    tileTextureDimension = 32;
    tileWorldDimension = 32;

	// Preallocate
	tiles.reserve(TILE_WIDTH * TILE_HEIGHT * 2);



    uint16_t c = 0;
    for(uint16_t y = 0; y < MAP_HEIGHT; y++)
    {
        for(uint16_t x = 0; x < MAP_WIDTH; x++)
        {

            int tId = (int)tileIDs[c] - 1;
            json tileData = tilesData[std::to_string(tId + 1)];
            tiles.push_back(Tile(x, y, TILE_WIDTH, TILE_HEIGHT, *this, (bool)tileData["walkable"], (bool)tileData["harvestable"], (uint16_t)tileData["resources"]));
            assert(!tiles.empty());
            Tile &tile = tiles.back();
            tile.tileID = tId;
            tile.id = c;
            tile.name = tileData["name"];
            tile.harvestable = tileData["harvestable"];
            tile.walkable = tileData["walkable"];
            tile.swimable = tileData["swimable"];
            tile.oilYield =  tileData["oil_yield"];
            tile.lumberYield = tileData["lumber_yield"];
            tile.goldYield = tileData["gold_yield"];
			tile.setResources(tileData["resources"]);
            tile.depleteTile = tileData["deplete_tile"];

            if(tile.name == "Spawn"){
                spawnTiles.push_back(c);
                // Replace spawn data with grass
                tId = Constants::Tile::Grass;
                json tileData = tilesData[std::to_string(tId - 1)];
                tId -= 2; // TODO some fucked up shit goin on here
                tile.id = c;
                tile.tileID = tId;
                tile.name = tileData["name"];
                tile.harvestable = tileData["harvestable"];
                tile.walkable = tileData["walkable"];
                tile.swimable = tileData["swimable"];
                tile.oilYield =  tileData["oil_yield"];
                tile.lumberYield = tileData["lumber_yield"];
                tile.goldYield = tileData["gold_yield"];
                tile.setResources(tileData["resources"]);
                tile.depleteTile = tileData["deplete_tile"];
            }

            c++;

        }
    }

}



std::vector<Tile> &Tilemap::getTiles() {
	return tiles;
}


std::vector<Tile *> Tilemap::neighbors(Tile &tile, Constants::Pathfinding type) {
    std::vector<Tile *> neighbors;

    std::pair<int,int> pos[8];

    pos[0].first = -1;
    pos[0].second = -1;

    pos[1].first = 0;
    pos[1].second = -1;

    pos[2].first = 1;
    pos[2].second = -1;

    pos[3].first = -1;
    pos[3].second = 0;

    pos[4].first = 1;
    pos[4].second = 0;

    pos[5].first = -1;
    pos[5].second = 1;

    pos[6].first = 0;
	pos[6].second = 1;

	pos[7].first = 1;
	pos[7].second = 1;

    //for(int i = 0; i < 1; i++){ // TODO width of neighbor
    for(auto &i : pos){
        int x = tile.x + i.first;
        int y = tile.y + i.second;

        if(x < 0 or y < 0 or x > MAP_WIDTH-1 or y > MAP_WIDTH-1)
            continue;

        int idx = MAP_WIDTH*y + x;

        Tile &neigh = tiles[idx];
        assert(&neigh);

		if (type == Constants::Pathfinding::All) {
			neighbors.push_back(&neigh);
			continue;
		}
		else if (type == Constants::Pathfinding::Walkable && neigh.isWalkable()) {
			neighbors.push_back(&neigh);
		}
		else if (type == Constants::Pathfinding::Attackable && neigh.isAttackable(*tile.getOccupant())) {
			neighbors.push_back(&neigh);
		}
		else if (type == Constants::Pathfinding::Harvestable && neigh.isHarvestable()) {
			neighbors.push_back(&neigh);
		}
		else {
			continue;
			//assert(false && "No type for neighbour");
		}
        
    }
    //}


    return neighbors;


}

Tile *Tilemap::getTile(int x, int y){
    int idx = MAP_WIDTH*y + x;
    return &tiles[idx];
}

std::vector<Tile *> Tilemap::getTiles(Tile *source, int width, int height) {
    /// Get tiles based on width and height of unit
    std::vector<Tile *> tiles;
    for (int _x = 0; _x < width; _x++) {
        for(int _y = 0; _y < height; _y++) {

            int x = source->x + _x;
            int y = source->y + _y;

            Tile *tile = getTile(x, y);
            tiles.push_back(tile);
        }
    }

    return tiles;
}


bool Tilemap::operator()(unsigned x, unsigned y) const
{
	if (x < MAP_WIDTH && y < MAP_HEIGHT) // Unsigned will wrap if < 0
	{
		int idx = MAP_WIDTH*y + x;
		const Tile &t = tiles[idx];
		const bool isWalk = t.isWalkable();
		return isWalk;
	}
}