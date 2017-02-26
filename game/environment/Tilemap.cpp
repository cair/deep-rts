//
// Created by Per-Arne on 24.02.2017.
//
#include "./Tile.h"
#include "Tilemap.h"
#include "../Constants.h"
#include "../unit/Unit.h"

Tilemap::Tilemap(json tilesData, json mapData, sf::Texture tileset_) {
    json tilesetData = mapData["tilesets"][0];
    json mapLayer = mapData["layers"][0];
    json tileIDs = mapLayer["data"];

    int mapWidth = mapData["width"];
    int mapHeight = mapData["height"];
    int n_cols = tilesetData["columns"];
    int tWidth = tilesetData["tilewidth"];
    int tHeight = tilesetData["tileheight"];
    int tFirstGid = tilesetData["firstgid"];

    tileset = tileset_;
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
            tiles.push_back(Tile(x, y, TILE_WIDTH, TILE_HEIGHT));
            assert(!tiles.empty());
            Tile &tile = tiles.back();
            tile.id = tId;
            tile.name = tileData["name"];
            tile.harvestable = tileData["harvestable"];
            tile.walkable = tileData["walkable"];
            tile.swimable = tileData["swimable"];
            tile.oilYield =  tileData["oil_yield"];
            tile.lumberYield = tileData["lumber_yield"];
            tile.goldYield = tileData["gold_yield"];
            tile.resources = tileData["resources"];

            if(tile.name == "Spawn"){
                spawnTiles.push_back(c);
                // Replace spawn data with grass
                tId = Constants::Tile_Grass;
                json tileData = tilesData[std::to_string(tId - 1)];
                tId -= 2; // TODO some fucked up shit goin on here
                tile.id = tId;
                tile.name = tileData["name"];
                tile.harvestable = tileData["harvestable"];
                tile.walkable = tileData["walkable"];
                tile.swimable = tileData["swimable"];
                tile.oilYield =  tileData["oil_yield"];
                tile.lumberYield = tileData["lumber_yield"];
                tile.goldYield = tileData["gold_yield"];
                tile.resources = tileData["resources"];

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

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &tileset;

    for(const Tile& tile : tiles){

        // Draw the tile itself
        target.draw(tile.vertices, 4, sf::Quads, states);
    }

}

Tilemap::Tilemap() {

}
