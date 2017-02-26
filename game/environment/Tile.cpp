//
// Created by Per-Arne on 24.02.2017.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "../player/Player.h"
#include "../unit/Unit.h"


Tile::Tile(int x, int y, int width, int height):
x(x), y(y), height(height), width(width)
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
    return false;
}

void Tile::setOccupant(Unit &unit) {
    occupant = &unit;
}


