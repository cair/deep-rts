//
// Created by per on 3/16/17.
//

#include "GraphicTile.h"
#include "GraphicTilemap.h"
#include <iostream>

GraphicTile::GraphicTile(Tile &dataTile, GraphicTilemap &tilemap):
        dataTile(dataTile),
        tilemap(tilemap)
{

    setupVerticles(dataTile.tileID, dataTile.width, dataTile.height, 1); // TODO firstgid always 1 ?
}


void GraphicTile::setupVerticles(unsigned tId, unsigned tileWidth, unsigned tileHeight, unsigned firstgid)
{

    unsigned gid = tId + 1;
    gid -= (firstgid - gid) > 0;
    sf::Vector2u size = tilemap.tileset.getSize();
    unsigned mod = gid % (size.x / tileWidth);
    unsigned div = gid / (size.x / tileWidth);

    vertices[0].position = sf::Vector2f(dataTile.x * tileWidth, dataTile.y * tileHeight);
    vertices[1].position = sf::Vector2f((dataTile.x + 1) * tileWidth, dataTile.y * tileHeight);
    vertices[2].position = sf::Vector2f((dataTile.x + 1) * tileWidth, (dataTile.y + 1) * tileHeight);
    vertices[3].position = sf::Vector2f(dataTile.x * tileWidth, (dataTile.y + 1) * tileHeight);

    vertices[0].texCoords = sf::Vector2f((mod * tileWidth) + mod, (div * tileHeight) + div);
    vertices[1].texCoords = sf::Vector2f(((mod + 1) * tileWidth) + mod, (div * tileHeight) + div);
    vertices[2].texCoords = sf::Vector2f(((mod + 1) * tileWidth) + mod, ((div + 1) * tileHeight) + div);
    vertices[3].texCoords = sf::Vector2f((mod * tileWidth) + mod, ((div + 1) * tileHeight) + div);
}


sf::Vector2f GraphicTile::getPixelPosition()const {
    return sf::Vector2f(vertices->position.x, vertices->position.y);
}
