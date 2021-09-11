//
// Created by Per-Arne on 24.02.2017.
//
#include "Tile.h"

#include <utility>
#include "Player.h"
#include "Game.h"
using namespace DeepRTS;

Tile::Tile(
		Tilemap &tilemap,
		int id,
		int x,
		int y,
		int width,
		int height,
        std::string newName,
		int newTypeID,
		bool newHarvestable,
        bool newWalkable,
        double newWalkModifier,
		int newResources,
        std::string depletedName,
		int depletedTypeID,
		bool depletedHarvestable,
		bool depletedWalkable,
		double depletedWalkModifier,
		int depletedResources,
        double lumberYield,
        double goldYield,
        double stoneYield,
        double newDamageModifier,
        double depletedDamageModifier
):
newHarvestable(newHarvestable),

newWalkable(newWalkable),

newWalkModifier(newWalkModifier),

newTypeId(newTypeID),

newResources(newResources),

newName(std::move(newName)),

newDamageModifier(newDamageModifier),

depletedHarvestable(depletedHarvestable),

depletedWalkable(depletedWalkable),

depletedWalkModifier(depletedWalkModifier),

depletedTypeId(depletedTypeID),

depletedResources(depletedResources),

depletedName(std::move(depletedName)),

depletedDamageModifier(depletedDamageModifier),

tilemap(tilemap),

harvestable(newHarvestable),

walkable(newWalkable),

walkModifier(newWalkModifier),

resources(newResources),

name(this->newName),

typeId(newTypeID),

depleted(false),

damageModifier(newDamageModifier),

id(id),

x(x),

y(y),

width(width),

height(height),

lumberYield(lumberYield),

goldYield(goldYield),

stoneYield(stoneYield)

{

	// Update state map
	tilemap.game.state(x, y, 0) = Constants::TypeToID.at(name);

	// Update state matrix
	if(resources <= 0) {
		setDepleted();
	}

	reset();
}


bool Tile::hasOccupant() const
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

int Tile::getResources() const
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

int Tile::getOccupantID() const {
	return occupantID;
}



bool Tile::isBuildable() const {
	return isWalkable();
}


int Tile::distance(Tile &target) const {
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
    walkModifier = newWalkModifier;
    resources = newResources;
    name = newName;
    typeId = newTypeId;
    depleted = false;
    damageModifier = newDamageModifier;

	tilemap.game.state(x, y, 0) = Constants::TypeToID.at(name);
    if(resources <= 0) {
        setDepleted();
    }

    triggerOnTileChange();
}

void Tile::setDepleted() {
	depleted = true;
    harvestable = depletedHarvestable;
    walkable = depletedWalkable;
    walkModifier = depletedWalkModifier;
    resources = depletedResources;
    name = depletedName;
    typeId = depletedTypeId;



	tilemap.game.state(x, y, 0) = Constants::TypeToID.at(name);
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

Position Tile::getPosition() const {
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

[[maybe_unused]] int Tile::getDepleteTile() const {
	return depletedTypeId;
}

double Tile::getWalkModifier() const {
    return walkModifier;
}

void Tile::update(Unit& unit) const {

    // TODO - does not handle multi-tile blocks (for example Town-Hall)
    // TODO - maybe this is OK?
    if(hasOccupant() && damageModifier > 0){
        unit.afflictDamage(std::ceil(damageModifier * tilemap.game.config.getTileDamageModifier()));
    }

}





