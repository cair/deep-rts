//
// Created by Per-Arne on 24.02.2017.
//
#include "Tilemap.h"

#include "../unit/Unit.h"
#include "../Game.h"
#include "../loaders/ResourceLoader.h"

Tilemap::Tilemap(std::string mapName, Game &game): game_(game){
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
    int tFirstGid = tilesetData["firstgid"].GetInt();

    TILE_WIDTH = tWidth;
    TILE_HEIGHT = tHeight;
    MAP_WIDTH = mapWidth;
    MAP_HEIGHT = mapHeight;


	// Preallocate
	tiles.reserve(TILE_WIDTH * TILE_HEIGHT * 2);



    uint16_t c = 0;
    for(uint16_t y = 0; y < MAP_HEIGHT; y++)
    {
        for(uint16_t x = 0; x < MAP_WIDTH; x++)
        {

            int newTypeId = tileIDs[c].GetInt() - 1;
            auto newTileData = tilesData[std::to_string(newTypeId + 1).c_str()].GetObject();
            int depletedTypeId = newTileData["deplete_tile"].GetInt();
            auto depletedTileData = tilesData[std::to_string(depletedTypeId).c_str()].GetObject();

            auto newName = std::string(newTileData["name"].GetString());
            auto depletedName = std::string(depletedTileData["name"].GetString());

            auto newWalkable = newTileData["walkable"].GetBool();
            auto depletedWalkable = depletedTileData["walkable"].GetBool();

            auto newHarvestable = newTileData["harvestable"].GetBool();
            auto depletedHarvestable = depletedTileData["harvestable"].GetBool();

            auto newSwimable = newTileData["swimable"].GetBool();
            auto depletedSwimable = depletedTileData["swimable"].GetBool();

            auto newResources = newTileData["resources"].GetUint();
            auto depletedResources = depletedTileData["resources"].GetUint();

            auto lumberYield = newTileData["lumber_yield"].GetInt();
            auto goldYield = newTileData["gold_yield"].GetInt();
            auto oilYield = newTileData["oil_yield"].GetInt();


            tiles.push_back(Tile(
                *this,
                c,
                x,
                y,
                TILE_WIDTH,
                TILE_HEIGHT,
                newName,
                newTypeId,
                newHarvestable,
                newWalkable,
                newSwimable,
                newResources,
                depletedName,
                depletedTypeId,
                depletedHarvestable,
                depletedWalkable,
                depletedSwimable,
                depletedResources,
                lumberYield,
                goldYield,
                oilYield));


            assert(!tiles.empty());
            Tile &tile = tiles.back();

            if(tile.getTypeId() == Constants::Tile::Spawn){
                spawnTiles.push_back(c);
                tile.setDepleted();
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

Tile &Tilemap::getTile(int x, int y){
    int idx = MAP_WIDTH*y + x;
    Tile &t = tiles[idx];
    return t;
}

std::vector<Tile *> Tilemap::getTileArea(Tile &source, int width, int height) {
    /// Get tiles based on width and height of unit
    std::vector<Tile *> tiles;
    for (int _x = 0; _x < width; _x++) {
        for(auto _y = 0; _y < height; _y++) {

            int x = source.x + _x;
            int y = source.y + _y;

            Tile &tile = getTile(x, y);
            tiles.push_back(&tile);
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

Game &Tilemap::getGame() const {
    return game_;
}
