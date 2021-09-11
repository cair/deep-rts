//
// Created by per on 04.02.18.
//

#ifndef DEEPRTS_MAP_H
#define DEEPRTS_MAP_H

#include <utility>
#include <vector>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
namespace DeepRTS {

    struct TileData {
        int depleteTile;
        std::string name;
        bool walkable;
        bool harvestable;
        double walk_modifier;
        int resources;
        double lumber_yield;
        double gold_yield;
        double stone_yield;
        double damage_modifier;

        TileData() = default;

        TileData(
                int depleteTile,
                std::string name,
                bool walkable,
                double walk_modifier,
                bool harvestable,
                int resources,
                double lumber_yield,
                double gold_yield,
                double stone_yield,
                double damage_modifier) : depleteTile(depleteTile), name(std::move(name)), walkable(walkable),
                                          harvestable(harvestable),
                                          walk_modifier(walk_modifier), resources(resources),
                                          lumber_yield(lumber_yield), gold_yield(gold_yield),
                                          stone_yield(stone_yield), damage_modifier(damage_modifier) {

        }
    };


    class Map {

    public:

        explicit Map(const std::string &map_file);

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

        std::map<int, TileData> tilesData;

    };

}
#endif //DEEPRTS_MAP_H
