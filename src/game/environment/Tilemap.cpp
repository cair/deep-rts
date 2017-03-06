//
// Created by Per-Arne on 24.02.2017.
//
#include <SFML/Graphics/RectangleShape.hpp>
#include "./Tile.h"
#include "Tilemap.h"
#include "../Constants.h"
#include "../unit/Unit.h"

Tilemap::Tilemap(std::string mapName) {
	json mapData = MapLoader::loadFile(mapName);

	json tilesData = MapLoader::tileProperties();
	tileset = MapLoader::loadTexture();


    json tilesetData = mapData["tilesets"][0];
    json mapLayer = mapData["layers"][0];
    json tileIDs = mapLayer["data"];

    int mapWidth = mapData["width"];
    int mapHeight = mapData["height"];
    int tWidth = tilesetData["tilewidth"];
    int tHeight = tilesetData["tileheight"];
    tFirstGid = tilesetData["firstgid"];

    TILE_WIDTH = tWidth;
    TILE_HEIGHT = tHeight;
    MAP_WIDTH = mapWidth;
    MAP_HEIGHT = mapHeight;
    tileTextureDimension = 32;
    tileWorldDimension = 32;

    int c = 0;
    for(int y = 0; y < MAP_HEIGHT; y++)
    {
        for(int x = 0; x < MAP_WIDTH; x++)
        {

            int tId = (int)tileIDs[c] - 1;
            json tileData = tilesData[std::to_string(tId + 1)];
            tiles.push_back(Tile(x, y, TILE_WIDTH, TILE_HEIGHT, *this));
            assert(!tiles.empty());
            Tile &tile = tiles.back();
            tile.tId = tId;
            tile.id_ = c;
            tile.name = tileData["name"];
            tile.harvestable = tileData["harvestable"];
            tile.walkable = tileData["walkable"];
            tile.swimable = tileData["swimable"];
            tile.oilYield =  tileData["oil_yield"];
            tile.lumberYield = tileData["lumber_yield"];
            tile.goldYield = tileData["gold_yield"];
            tile.resources = tileData["resources"];
            tile.depleteTile = tileData["deplete_tile"];

            if(tile.name == "Spawn"){
                spawnTiles.push_back(c);
                // Replace spawn data with grass
                tId = Constants::Tile_Grass;
                json tileData = tilesData[std::to_string(tId - 1)];
                tId -= 2; // TODO some fucked up shit goin on here
                tile.id_ = c;
                tile.tId = tId;
                tile.name = tileData["name"];
                tile.harvestable = tileData["harvestable"];
                tile.walkable = tileData["walkable"];
                tile.swimable = tileData["swimable"];
                tile.oilYield =  tileData["oil_yield"];
                tile.lumberYield = tileData["lumber_yield"];
                tile.goldYield = tileData["gold_yield"];
                tile.resources = tileData["resources"];
                tile.depleteTile = tileData["deplete_tile"];
            }


            addTileVertices(tId, tWidth, tHeight, tFirstGid, tile);

            c++;
        }
    }

}

void Tilemap::addTileVertices(unsigned tId, unsigned tileWidth, unsigned tileHeight, unsigned firstgid, Tile &tile)
{

    unsigned gid = tId + 1;
    gid -= (firstgid - gid) > 0;
    sf::Vector2u size = tileset.getSize();
    unsigned mod = gid % (size.x / tileWidth);
    unsigned div = gid / (size.x / tileWidth);

    tile.vertices[0].position = sf::Vector2f(tile.x * tileWidth, tile.y * tileHeight);
    tile.vertices[1].position = sf::Vector2f((tile.x + 1) * tileWidth, tile.y * tileHeight);
    tile.vertices[2].position = sf::Vector2f((tile.x + 1) * tileWidth, (tile.y + 1) * tileHeight);
    tile.vertices[3].position = sf::Vector2f(tile.x * tileWidth, (tile.y + 1) * tileHeight);

    tile.vertices[0].texCoords = sf::Vector2f((mod * tileWidth) + mod, (div * tileHeight) + div);
    tile.vertices[1].texCoords = sf::Vector2f(((mod + 1) * tileWidth) + mod, (div * tileHeight) + div);
    tile.vertices[2].texCoords = sf::Vector2f(((mod + 1) * tileWidth) + mod, ((div + 1) * tileHeight) + div);
    tile.vertices[3].texCoords = sf::Vector2f((mod * tileWidth) + mod, ((div + 1) * tileHeight) + div);
}




std::vector<Tile *> Tilemap::neighbors(Tile &tile, int const_pathfinding_type) {
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

        if(const_pathfinding_type == Constants::Pathfinding_Walkable && !neigh.canWalkTo())
            continue;

        neighbors.push_back(&neigh);
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
