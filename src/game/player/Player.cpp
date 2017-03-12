//
// Created by Per-Arne on 24.02.2017.
//


#include "Player.h"
#include "../unit/Peasant.h"
#include "../Game.h"
#include "../util/ColorConverter.hpp"
#include "../algorithms/RANDOM/AlgoRandom.h"
#include "../unit/TownHall.h"

int Player::gId = 0;

Player::Player(Game &game): inventoryManager(*this), game_(game) {
    id_ = Player::gId++;
    name_ = "Player: " + std::to_string(id_);
    playerColor = ColorConverter::hsv(fmod(Player::gId * 0.618033988749895, 1.0),
                                      0.5,
                                      sqrt(1.0 - fmod(Player::gId * 0.618033988749895, 0.5)));

	unitIndexes.reserve(1000);
    faction = 0;
    gold = 1500;
    lumber = 750;
    oil = 0;
    foodConsumption = 0;
    food = 1;

    statGoldGather = 0;
    statLumberGather = 0;
    statOilGather = 0;
    statUnitDamageDone = 0;
    statUnitDamageTaken = 0;
    statUnitBuilt = 0;
    statUnitMilitary = 0;

}


Unit& Player::spawn(Tile &spawnPoint) {
    // Spawn a builder


	Unit *unit = NULL;
    if(faction == 0) // Human
    {
        
        unit = &addUnit(Constants::Unit::Peasant);
    }
    else if(faction == 1) // Orc
    {
		unit = &addUnit(Constants::Unit::Peasant);
    }
    else {
        assert(false);
    }

	assert(unit && "Unit was null for some reason");
	unit->spawn(spawnPoint, unit->spawnDuration);


    return *unit;
}

void Player::update() {
	
	/*if (defeated && units.size() == 0)
		return;

    for(auto &unit : units) {
        unit->update();
    }
    std::vector<Unit &>::iterator it;
    for(it = units.begin(); it != units.end();) {
        (*it)->update();

        if((*it)->removedFromGame) {
            it = units.erase(it);
        }else {
            it++;
        }
    }

    if(algorithm_) {
        algorithm_->update();
    }
	*/

	if (!actionQueue.empty()) {
		int actionID = actionQueue.front();
		actionQueue.pop_front();

		switch(actionID) {
		case Constants::Action::NextUnit:
			nextUnit();
			break;
		case Constants::Action::PreviousUnit:
			previousUnit();
			break;
		case Constants::Action::RightUpRight:
			targetedUnit->rightClickRelative(-1, 1);
			break;
		case Constants::Action::RightUpLeft:
			targetedUnit->rightClickRelative(-1, -1);
			break;
		case Constants::Action::RightDownRight:
			targetedUnit->rightClickRelative(1, 1);
			break;
		case Constants::Action::RightDownLeft:
			targetedUnit->rightClickRelative(1, -1);
			break;
		case Constants::Action::RightUp:
			targetedUnit->rightClickRelative(0, -1);
			break;
		case Constants::Action::RightDown:
			targetedUnit->rightClickRelative(0, 1);
			break;
		case Constants::Action::RightLeft:
			targetedUnit->rightClickRelative(-1, 0);
			break;
		case Constants::Action::RightRight:
			targetedUnit->rightClickRelative(1, 0);
			break;
		case Constants::Action::Build0:
			targetedUnit->build(0);
			break;
		case Constants::Action::Build1:
			targetedUnit->build(1);
			break;
		case Constants::Action::Build2:
			targetedUnit->build(2);
			break;

		}


	}


	


}



int Player::getFoodConsumption() {
    return foodConsumption;
}

int Player::getFood() {
    return food;
}

int Player::getGold() {
    return gold;
}

int Player::getOil() {
    return oil;
}

int Player::getLumber() {
    return lumber;
}

size_t Player::getUnitCount() {
	return unitIndexes.size();
}

int Player::getId() {
    return id_;
}

void Player::addGold(int n) {
    gold += n;
}

void Player::addLumber(int n) {
    lumber += n;
}

void Player::addOil(int n) {
    oil += n;
}

int Player::getScore() {
	uint8_t GOLD_VALUE = 2;
	uint8_t LUMBER_VALUE = 1;
	uint8_t OIL_VALUE = 3;


    double_t gatherScore = (statGoldGather * GOLD_VALUE + statLumberGather * LUMBER_VALUE) * .5;
	double_t builtScore = statUnitBuilt * 10;
	double_t damageScore = std::max(0.0, statUnitDamageDone - (statUnitDamageTaken * .5));
	double_t unitScore = statUnitBuilt * 5;


	double_t militaryScore = 0;
	double_t defenceScore = 0;


    return static_cast<uint32_t>(gatherScore + builtScore + unitScore + militaryScore + defenceScore + damageScore);

}

void Player::removeUnit(Unit & unit) {
    for(auto &p : game_.players) {
        if(p.targetedUnit == &unit) {
            p.targetedUnit = NULL;
        }
    }

    unit.removedFromGame = true;


    //units.erase(std::remove(units.begin(), units.end(), unit), units.end());
    std::cout << "Implement removeUnit" << std::endl;

}

bool Player::checkDefeat(){
	int aliveUnits = 0;

	bool isDefeated = (unitIndexes.size() == 0);
    defeated = isDefeated;
	if (defeated) {
		name_ += " [DEFEATED]";
	}
	

    return defeated;
}

bool Player::canPlace(Unit & builder, Unit & unit, Tile *_tile) {
	
    for (auto &tile : game_.map.getTiles(_tile, unit.width, unit.height)) {
        if(tile == builder.tile) // Ignore tile of the builder, this is handled in Unit::Build when builder despawns
            continue;

        if(!tile->isBuildable()) {
            return false;
        }

    }

    return true;
}

bool Player::canAfford(Unit & unit) {
    return gold >= unit.goldCost and lumber >= unit.lumberCost and oil >= unit.oilCost;

}

Unit& Player::addUnit(Constants::Unit unitType) {

	if (unitType == Constants::Unit::Peasant) {
		game_.units.push_back(Peasant(*this));
	}
	else if (unitType == Constants::Unit::TownHall) {
		game_.units.push_back(TownHall(*this));
	}

	unitIndexes.push_back(game_.units.size() - 1);
	return game_.units.back();
}

void Player::subOil(int n) {
    oil -= n;
}

void Player::subLumber(int n) {
    lumber -= n;
}

void Player::subGold(int n) {
    gold -= n;
}

void Player::setName(std::string name){
    name_ = name;
}

void Player::setAlgorithm(std::shared_ptr<Algorithm> theAlg){
    algorithm_ = theAlg;
}

int Player::_getNextPrevUnitIdx(){
    if(unitIndexes.size() == 0){
        return -1;
    }
    if(!targetedUnit){
        return 0;
    }

    uint16_t idx = 0;
    for(auto &uIDX : unitIndexes){
        if(targetedUnit == &game_.getUnit(uIDX)){
            return idx;
        }
        idx++;
    }

	return -1;
}

void Player::nextUnit(){

	// Get which index in the unitIndexes to lookup
    int idx = _getNextPrevUnitIdx() + 1;

	// Just return if no next unit is returned
    if(idx == -1)
        return;

	// Increment since we want Next unit
	idx++;

	// Retrieve which index it has
	uint16_t unitIndex = unitIndexes[idx % unitIndexes.size()];
	std::cout << static_cast<int>(unitIndex) << " -N- " << unitIndexes.size() << std::endl;
	// Get unit from the game vector
	targetedUnit = &game_.getUnit(unitIndex);

	return;
}

void Player::previousUnit(){
	// Get which index in the unitIndexes to lookup
	int idx = _getNextPrevUnitIdx() + 1;

	// Just return if no next unit is returned
	if (idx == -1)
		return;

	// Increment since we want Previous unit
	idx--;

	// Retrieve which index it has
	uint16_t unitIndex = unitIndexes[idx % unitIndexes.size()];
	std::cout << static_cast<int>(unitIndex) << " -P- " << unitIndexes.size() << std::endl;

	// Get unit from the game vector
	targetedUnit = &game_.getUnit(unitIndex);

	return;
}

Unit Player::createUnit(int type_id) {
    switch (type_id){
        case Constants::Unit::Peasant:
			return Peasant(*this);
        case Constants::Unit::TownHall:
			return TownHall(*this);


    }
}

void Player::queueAction(int actionID) {
	actionQueue.push_back(actionID);
}