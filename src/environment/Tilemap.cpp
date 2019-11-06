//
// Created by Per-Arne on 24.02.2017.
//
#include "Tilemap.h"

#include "../unit/Unit.h"
#include "../Game.h"
#include "../loaders/ResourceLoader.h"

Tilemap::Tilemap(Map& map, Game &game): game(game){


	// Preallocate
	tiles.reserve(map.MAP_HEIGHT * map.MAP_WIDTH * 2);


    int c = 0;
    for(int y = 0; y < map.MAP_HEIGHT; y++)
    {
        for(int x = 0; x < map.MAP_WIDTH; x++)
        {
            int newTypeId = map.tileIDs[c];

            auto newTileData = map.tilesData[newTypeId];
            int depletedTypeId = newTileData.depleteTile;
            auto depletedTileData = map.tilesData[depletedTypeId];

            auto newName = newTileData.name;
            auto depletedName = depletedTileData.name;

            auto newWalkable = newTileData.walkable;
            auto depletedWalkable = depletedTileData.walkable;

            auto newHarvestable = newTileData.harvestable;
            auto depletedHarvestable = depletedTileData.harvestable;

            auto newSwimable = newTileData.swimable;
            auto depletedSwimable = depletedTileData.swimable;

            auto newResources = newTileData.resources;
            auto depletedResources = depletedTileData.resources;

            auto lumberYield = newTileData.lumber_yield;
            auto goldYield = newTileData.gold_yield;
            auto oilYield = newTileData.oil_yield;

                      tiles.emplace_back(Tile(
                *this,
                c,
                x,
                y,
                map.TILE_WIDTH,
                map.TILE_HEIGHT,
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

            //Tile &tile = tiles.back();
            if(newTypeId == Constants::Tile::Spawn){
                spawnTiles.push_back(c);
            }

            c++;

        }
    }
}

std::vector<Tile> &Tilemap::getTiles() {
	return tiles;
}

std::vector<Tile *> Tilemap::neighbors(Tile &tile, Constants::Pathfinding type) {
    // TODO slow as fuck
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

        if(x < 0 || y < 0 || x > game.map.MAP_WIDTH-1 || y > game.map.MAP_WIDTH-1)
            continue;

        int idx = game.map.MAP_WIDTH*y + x;

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
    assert(x >= 0);
    assert(y >= 0);
    int idx = game.map.MAP_WIDTH*y + x;
    Tile &t = tiles[idx];
    return t;
}

std::vector<Tile *> Tilemap::getTileArea(Tile &source, int width, int height) {
    /// Get tiles based on width and height of unit
    std::vector<Tile *> tiles_;
    for (int _x = 0; _x < width; _x++) {
        for(auto _y = 0; _y < height; _y++) {

            int x = source.x + _x;
            int y = source.y + _y;

            Tile &tile = getTile(x, y);
            tiles_.push_back(&tile);
        }
    }

    return tiles_;
}

void Tilemap::reset() {
    // Reset all tiles
    for (auto &tile : tiles) {
        tile.reset();
    }
}


bool Tilemap::operator()(unsigned x, unsigned y) const
{

	if ((int)x >= game.map.MAP_WIDTH || (int)y >= game.map.MAP_HEIGHT) // Unsigned will wrap if < 0
    {
	    return false;
    }

	int idx = game.map.MAP_WIDTH * (int)y + (int)x;
	const Tile &t = tiles[idx];
	const bool isWalk = t.isWalkable();
	//std::cout << isWalk << " - " <<  x << "," << y << std::endl;
	return isWalk;

}


