//
// Created by per on 04.02.18.
//

#include "Map.h"
#include <string>
#include "../loaders/ResourceLoader.h"




Map::Map(const std::string& map_file): mapFile(map_file) {
    ResourceLoader::getInstance().loadMapJSON(map_file);

    auto mapData = ResourceLoader::getInstance().mapJSON.GetObject();
    auto _tilesData = ResourceLoader::getInstance().tileJSON.GetObject();



    auto tilesetData = mapData["tilesets"].GetArray()[0].GetObject();
    auto mapLayer = mapData["layers"].GetArray()[0].GetObject();
    auto _tileIDs = mapLayer["data"].GetArray();

    for(auto &id : _tileIDs) {
        int tileId = id.GetInt();
        tileIDs.emplace_back(tileId);
    }

    for(auto tileId : tileIDs) {
        auto tileData = _tilesData[std::to_string(tileId).c_str()].GetObject();

        tilesData.emplace(tileId,TileData(
                tileData["deplete_tile"].GetInt(),
                tileData["name"].GetString(),
                tileData["walkable"].GetBool(),
                tileData["harvestable"].GetBool(),
                tileData["swimable"].GetBool(),
                tileData["resources"].GetInt(),
                tileData["lumber_yield"].GetInt(),
                tileData["gold_yield"].GetInt(),
                tileData["oil_yield"].GetInt())
        );


        /*auto newTileData = _tilesData[std::to_string(tileId).c_str()].GetObject();
        int depletedTypeId = newTileData["deplete_tile"].GetInt();
        auto depletedTileData = map.tilesData[std::to_string(depletedTypeId).c_str()].GetObject();

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
        auto oilYield = newTileData["oil_yield"].GetInt();*/



    }

    int mapWidth = mapData["width"].GetInt();
    int mapHeight = mapData["height"].GetInt();
    int tWidth = tilesetData["tilewidth"].GetInt();
    int tHeight = tilesetData["tileheight"].GetInt();
    //int tFirstGid = tilesetData["firstgid"].GetInt();

    TILE_WIDTH = tWidth;
    TILE_HEIGHT = tHeight;
    MAP_WIDTH = mapWidth;
    MAP_HEIGHT = mapHeight;


}
