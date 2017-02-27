//
// Created by Per-Arne on 24.02.2017.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "../player/Player.h"
#include "../unit/Unit.h"
#include <utility>

Tile::Tile(int x, int y, int width, int height, Tilemap& tilemap):
x(x), y(y), height(height), width(width), tilemap(tilemap)
{

}


const sf::Vector2f Tile::getPixelPosition() {

    return sf::Vector2f(vertices->position.x, vertices->position.y);
}

bool Tile::isAttackable(Unit &unit) {
    if (!occupant)
        return false;

    return occupant->player_.getId() != unit.player_.getId();

}

bool Tile::isWalkable() {
    return walkable;
}

bool Tile::isHarvestable() {
    return harvestable;
}

void Tile::setOccupant(Unit &unit) {
    occupant = &unit;
}

bool Tile::canWalkTo() {
    return occupant == 0 and walkable;
}

bool Tile::isBuildable() {
    return canWalkTo();
}


int Tile::distance(Tile *pTile) {
    return abs(pTile->x - x) + abs(pTile->y - y);
}

void Tile::setDepleted() {
    this->harvestable = false;
    this->walkable = true;

    tilemap.addTileVertices(depleteTile-1, width, height, tilemap.tFirstGid, *this);

}





