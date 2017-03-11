//
// Created by Per-Arne on 24.02.2017.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "../player/Player.h"
#include "../unit/Unit.h"
#include <utility>

Tile::Tile(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Tilemap &tilemap):
x(x), y(y), height(height), width(width), tilemap(tilemap)
{}


sf::Vector2f Tile::getPixelPosition()const {
    return sf::Vector2f(vertices->position.x, vertices->position.y);
}

std::shared_ptr<Unit> Tile::getOccupant()
{
	return occupant;
}

uint16_t Tile::getResources()
{
	return resources;
}

bool Tile::isAttackable(std::shared_ptr<Unit> unit) {
    if (!occupant)
        return false;

    return occupant->player_.getId() != unit->player_.getId();

}

bool Tile::isWalkable() const{
    return walkable;
}

bool Tile::isHarvestable() const {
    return harvestable;
}

void Tile::setOccupant(std::shared_ptr<Unit> unit) {
    occupant = unit;
	needRedraw = true;
}

bool Tile::canWalkTo() const {
    return occupant == 0 and walkable;
}

bool Tile::isBuildable() const {
    return canWalkTo();
}


uint16_t Tile::distance(Tile *pTile) {
	return abs(pTile->x - x) + abs(pTile->y - y);
}

void Tile::setDepleted() {
    this->harvestable = false;
    this->walkable = true;
    tilemap.addTileVertices(depleteTile-1, width, height, tilemap.tFirstGid, *this);
}

void Tile::setResources(uint16_t resource_count)
{
	resources = resource_count;
}

void Tile::takeResource(uint8_t n) {
	resources -= n;
}





