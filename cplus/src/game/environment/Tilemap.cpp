//
// Created by Per-Arne on 24.02.2017.
//
#include "Tilemap.h"

#include "../unit/Unit.h"
#include "../Game.h"
#include "../loaders/ResourceLoader.h"

Tilemap::Tilemap(std::string mapName, Game *game): game_(game){
    ResourceLoader::getInstance().loadMapJSON(mapName);

	auto mapData = ResourceLoader::getInstance().mapJSON.GetObject();
	auto tilesData = ResourceLoader::getInstance().tileJSON.GetObject();



    auto tilesetData = mapData["tilesets"].GetArray()[0].GetObject();
    auto mapLayer = mapData["layers"].GetArray()[0].GetObject();
    auto tileIDs = mapLayer["data"].GetArray();

    int mapWidth = mapData["width"].GetInt();
    int mapHeight = mapData["height"].GetInt();
    int tWidth = tilesetData["tilewidth"].GetInt();
    int tHeight = tilesetData["tileheight"].GetInt();
    tFirstGid = tilesetData["firstgid"].GetInt();

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

            int tId = tileIDs[c].GetInt() - 1;
            auto tileData = tilesData[std::to_string(tId + 1).c_str()].GetObject();

            tiles.push_back(
                    Tile(x,
                         y,
                         TILE_WIDTH,
                         TILE_HEIGHT,
                         *this,
                         tileData["walkable"].GetBool(),
                         tileData["harvestable"].GetBool(),
                         tileData["resources"].GetUint()));
            assert(!tiles.empty());
            Tile &tile = tiles.back();
            tile.tileID = tId;
            tile.id = c;
            tile.name = tileData["name"].GetString();
            tile.harvestable = tileData["harvestable"].GetBool();
            tile.walkable = tileData["walkable"].GetBool();
            tile.swimable = tileData["swimable"].GetBool();
            tile.oilYield =  tileData["oil_yield"].GetUint();
            tile.lumberYield = tileData["lumber_yield"].GetUint();
            tile.goldYield = tileData["gold_yield"].GetUint();
			tile.setResources(tileData["resources"].GetUint64());
            tile.depleteTile = tileData["deplete_tile"].GetUint();

            if(tile.name == "Spawn"){
                spawnTiles.push_back(c);
                // Replace spawn data with grass
                tId = Constants::Tile::Grass;
                auto tileData = tilesData[std::to_string(tId - 1).c_str()].GetObject();
                tId -= 2; // TODO some fucked up shit goin on here
                tile.id = c;
                tile.tileID = tId;
                tile.name = tileData["name"].GetString();
                tile.harvestable = tileData["harvestable"].GetBool();
                tile.walkable = tileData["walkable"].GetBool();
                tile.swimable = tileData["swimable"].GetBool();
                tile.oilYield =  tileData["oil_yield"].GetUint();
                tile.lumberYield = tileData["lumber_yield"].GetUint();
                tile.goldYield = tileData["gold_yield"].GetUint();
                tile.setResources(tileData["resources"].GetUint64());
                tile.depleteTile = tileData["deplete_tile"].GetUint();
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

    //for(auto i = 0; i < 1; i++){ // TODO width of neighbor
    for(auto &i : pos){
        int x = tile.x + i.first;
        int y = tile.y + i.second;

        if(x < 0 || y < 0 || x > MAP_WIDTH-1 || y > MAP_WIDTH-1)
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
        for(auto _y = 0; _y < height; _y++) {

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

    return false;
}