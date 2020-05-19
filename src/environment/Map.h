//
// Created by per on 04.02.18.
//

#ifndef DEEPRTS_MAP_H
#define DEEPRTS_MAP_H

#include <utility>
#include <vector>
#include <iostream>
#include <map>
#include "../../../include/rapidjson/include/rapidjson/document.h"

struct TileData {
    int depleteTile;
    std::string name;
    bool walkable;
    bool harvestable;
    bool swimable;
    int resources;
    int lumber_yield;
    int gold_yield;
    int oil_yield;

    TileData()= default;

    TileData(
            int depleteTile,
            std::string name,
            bool walkable,
            bool harvestable,
            bool swimable,
            int resources,
            int lumber_yield,
            int gold_yield,
            int oil_yield) : depleteTile(depleteTile),
				     name(std::move(name)), 
				     walkable(walkable), 
				     harvestable(harvestable),
				     swimable(swimable), 
				     resources(resources), 
				     lumber_yield(lumber_yield), 
				     gold_yield(gold_yield),
				     oil_yield(oil_yield) {} 
};

struct SpawnTile {
    int unit_to_spawn;
    int unit_owner_player;

    SpawnTile()= default;

    SpawnTile(int u_to_spawn, 
            int u_owner_player): unit_to_spawn(u_to_spawn),
                                 unit_owner_player(u_owner_player) {}
};


class Map {

public:

    explicit Map(const std::string& map_file);

    std::string mapFile;

    /// A tile's Width
    int TILE_WIDTH;

    /// A tile's Height
    int TILE_HEIGHT;

    /// The Map Width (Num of tiles in width)
    int MAP_WIDTH;

    /// The Map Height (Num of tiles in height)
    int MAP_HEIGHT;

    std::vector<int> tileIDs;

    std::vector<SpawnTile> spawnTiles;

    std::map<int, TileData> tilesData;

};


#endif //DEEPRTS_MAP_H
