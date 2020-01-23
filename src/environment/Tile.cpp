//
// Created by Per-Arne on 24.02.2017.
//
#include "Tile.h"
#include "../player/Player.h"
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

harvestable(newHarvestable),

walkable(newWalkable),

swimable(newSwimable),

resources(newResources),

name(newName),

typeId(newTypeID),

newHarvestable(newHarvestable),

newWalkable(newWalkable),

newSwimable(newSwimable),

newTypeId(newTypeID),

newResources(newResources),

newName(newName),

depletedHarvestable(depletedHarvestable),

depletedWalkable(depletedWalkable),

depletedSwimable(depletedSwimable),

depletedTypeId(depletedTypeID),

depletedResources(depletedResources),

depletedName(depletedName),

id(id),

x(x),

y(y),

width(width),

height(height),

lumberYield(lumberYield),

goldYield(goldYield),

oilYield(oilYield)
{

	// Update state map
	tilemap.game.state(x, y, 0) = Constants::TypeToID.at(typeId);

	// Update state matrix
	if(resources <= 0) {
		setDepleted();
	}

	reset();
}


bool Tile::hasOccupant()
{
    return occupantID != -1;
}


Unit* Tile::getOccupant()
{
	if(occupantID == -1) {
		return nullptr;
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
	}else{
	    occupantID = unit->id;
	}

	setOccupantID(occupantID);
}

void Tile::setOccupantID(int unitID) {
	occupantID = unitID;
	//std::cout << unitID << " - Change  - " << x << "," << y << std::endl;
	tilemap.game._onTileChange(*this);
}

int Tile::getOccupantID() {
	return occupantID;
}



bool Tile::isBuildable() const {
	return isWalkable();
}


int Tile::distance(Tile &target) {
    return (int)hypot(x - target.x, y - target.y);
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

	tilemap.game.state(x, y, 0) = Constants::TypeToID.at(typeId);
    if(resources <= 0) {
        setDepleted();
    }

    triggerOnTileChange();
}

void Tile::setDepleted() {
	depleted = true;
    harvestable = depletedHarvestable;
    walkable = depletedWalkable;
    swimable = depletedSwimable;
    resources = depletedResources;
    name = depletedName;
    typeId = depletedTypeId;



	tilemap.game.state(x, y, 0) = Constants::TypeToID.at(typeId);
    triggerOnTileChange();
}


void Tile::triggerOnTileChange(){
    tilemap.game._onTileChange(*this);
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

int Tile::getDepleteTile() const {
	return depletedTypeId;
}





