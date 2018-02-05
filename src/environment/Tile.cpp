//
// Created by Per-Arne on 24.02.2017.
//

#include <iostream>
#include "Tile.h"
#include "../player/Player.h"
#include "../unit/Unit.h"
#include "../Game.h"

Tile::Tile(
		Tilemap &tilemap,
		int id,
		int x,
		int y,
		int width,
		int height,
		std::string& newName,
		int newTypeID,
		bool newHarvestable,
		bool newWalkable,
		bool newSwimable,
		int newResources,
		std::string& depletedName,
		int depletedTypeID,
		bool depletedHarvestable,
		bool depletedWalkable,
		bool depletedSwimable,
		int depletedResources,
		int lumberYield,
		int goldYield,
		int oilYield
):
tilemap(tilemap),
id(id),
x(x),
y(y),
width(width),
height(height),
name(newName),
newName(newName),
typeId(newTypeID),
newTypeId(newTypeID),
harvestable(newHarvestable),
newHarvestable(newHarvestable),
walkable(newWalkable),
newWalkable(newWalkable),
swimable(newSwimable),
newSwimable(newSwimable),
resources(newResources),
newResources(newResources),
depletedName(depletedName),
depletedTypeId(depletedTypeID),
depletedHarvestable(depletedHarvestable),
depletedWalkable(depletedWalkable),
depletedSwimable(depletedSwimable),
depletedResources(depletedResources),
lumberYield(lumberYield),
goldYield(goldYield),
oilYield(oilYield)
{

	// Update state map
	tilemap.game.state(x, y, 0) = typeId;

	// Update state matrix
	if(resources <= 0) {
		setDepleted();
	}
}


bool Tile::hasOccupant()
{
    return occupantID != -1;
}


Unit* Tile::getOccupant()
{
	if(occupantID == -1) {
		return NULL;
	}

	Unit &occupant = tilemap.game.units[occupantID];
	return &occupant;
}

int Tile::getResources()
{
	return resources;
}

bool Tile::isAttackable(Unit &unit) {
	Unit *occupant = getOccupant();
    if (!occupant)
        return false;

    return occupant->player_.getId() != unit.player_.getId();

}

bool Tile::isWalkable() const{
	return occupantID == -1 && walkable;
}

bool Tile::isHarvestable() const {
    return harvestable;
}

void Tile::setOccupant(Unit* unit) {
	if(!unit) {
		occupantID = -1;
		return;
	}
    occupantID = unit->id;
}

int Tile::getOccupantID() {
	return occupantID;
}
void Tile::setOccupantID(int unitID) {
	occupantID = unitID;
}


bool Tile::isBuildable() const {
	return isWalkable();
}


int Tile::distance(Tile &target) {
    return hypot(x - target.x, y - target.y);
}

void Tile::reset()
{
	if(hasOccupant()) {
		getOccupant()->clearStateForTile(this);
	}
    occupantID = -1;
    harvestable = newHarvestable;
    walkable = newWalkable;
    swimable = newSwimable;
    resources = newResources;
    name = newName;
    typeId = newTypeId;
    depleted = false;

	tilemap.game.state(x, y, 0) = typeId;
    if(resources <= 0) {
        setDepleted();
    }

}

void Tile::setDepleted() {
	depleted = true;
    harvestable = depletedHarvestable;
    walkable = depletedWalkable;
    swimable = depletedSwimable;
    resources = depletedResources;
    name = depletedName;
    typeId = depletedTypeId;

	tilemap.game.state(x, y, 0) = typeId;
    tilemap.game._onTileDeplete(*this);
}

void Tile::setResources(uint16_t resource_count)
{
	resources = resource_count;
}

void Tile::takeResource(uint8_t n) {
	resources -= n;
}

Position Tile::getPosition() {
    return {x, y};
}

bool Tile::isDepleted() const {
	return depleted;
}

int Tile::getTypeId() const {
	return typeId;
}

Tilemap &Tile::getTilemap() const {
	return tilemap;
}

const std::string &Tile::getName() const {
	return name;
}





