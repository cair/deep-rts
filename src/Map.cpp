//
// Created by per on 04.02.18.
//
#include <string>
#include "Map.h"
#include "ResourceLoader.h"


using namespace DeepRTS;



Map::Map(const std::string& map_file): mapFile(map_file) {
    ResourceLoader::getInstance().loadMapJSON(map_file);



    auto mapData = ResourceLoader::getInstance().mapJSON;
    auto _tileData = ResourceLoader::getInstance().tileData;

    auto tilesetData = mapData["tilesets"][0];

    auto mapLayer = mapData["layers"][0];

    auto _tileIDs = mapLayer["data"];

    for(auto &id : _tileIDs) {
        int tileId = id.get<int>();
        tileIDs.emplace_back(tileId-1);
    }

    for(auto tileId : tileIDs) {
        auto tileData = _tileData.getTileData(tileId);
        if(tileData.is_null()){
            std::runtime_error("Could not load tile");
        }

        if(tileData["deplete_tile"].is_null()){std::cout << "Deplete Tile missing" << std::endl;}
        else if(tileData["name"].is_null()){std::cout << "Name missing" << std::endl;}
        else if(tileData["walkable"].is_null()){std::cout << "walkable missing" << std::endl;}
        else if(tileData["walk_modifier"].is_null()){std::cout << "walk_modifier missing" << std::endl;}
        else if(tileData["harvestable"].is_null()){std::cout << "harvestable missing" << std::endl;}
        else if(tileData["resources"].is_null()){std::cout << "resources" << std::endl;}
        else if(tileData["lumber_yield"].is_null()){std::cout << "lumber_yield" << std::endl;}
        else if(tileData["gold_yield"].is_null()){std::cout << "gold_yield" << std::endl;}
        else if(tileData["stone_yield"].is_null()){std::cout << "stone_yield" << std::endl;}

        auto deplete_tile = tileData["deplete_tile"].get<int>();
        auto name = tileData["name"].get<std::string>();
        auto walkable = tileData["walkable"].get<bool>();
        auto walk_modifier = tileData["walk_modifier"].get<double>();
        auto harvestable = tileData["harvestable"].get<bool>();
        auto resources = tileData["resources"].get<int>();
        auto lumber_yield = tileData["lumber_yield"].get<double>();
        auto gold_yield = tileData["gold_yield"].get<double>();
        auto stone_yield = tileData["stone_yield"].get<double>();
        auto damage_modifier = tileData["damage_modifier"].get<double>();

        tilesData.emplace(tileId,TileData(
                deplete_tile,
                name,
                walkable,
                walk_modifier,
                harvestable,
                resources,
                lumber_yield,
                gold_yield,
                stone_yield,
                damage_modifier
        ));

    }

    int mapWidth = mapData["width"].get<int>();
    int mapHeight = mapData["height"].get<int>();
    int tWidth = tilesetData["tilewidth"].get<int>();
    int tHeight = tilesetData["tileheight"].get<int>();
    //int tFirstGid = tilesetData["firstgid"].GetInt();

    TILE_WIDTH = tWidth;
    TILE_HEIGHT = tHeight;
    MAP_WIDTH = mapWidth;
    MAP_HEIGHT = mapHeight;


}
