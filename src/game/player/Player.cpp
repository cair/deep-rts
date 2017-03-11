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
    std::shared_ptr<Unit> unit;



    if(faction == 0) // Human
    {
        unit = std::shared_ptr<Unit>(new Peasant(*this));
        units.push_back(unit);
    }
    else if(faction == 1) // Orc
    {
        unit = std::shared_ptr<Unit>(new Peasant(*this));
        units.push_back(unit);
    }
    else {
        assert(false);
    }

    assert(!units.empty());
    unit->spawn(spawnPoint, unit->spawnDuration);



    return *unit;
}

void Player::update() {
	if (defeated && units.size() == 0)
		return;

    /*for(auto &unit : units) {
        unit->update();
    }*/
    std::vector<std::shared_ptr<Unit>>::iterator it;
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
    return units.size();
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
	double_t unitScore = units.size() * 5;


	double_t militaryScore = 0;
	double_t defenceScore = 0;


    return static_cast<uint32_t>(gatherScore + builtScore + unitScore + militaryScore + defenceScore + damageScore);

}

void Player::removeUnit(std::shared_ptr<Unit> unit) {
    for(auto p : game_.players) {
        if(p->targetedUnit == unit) {
            p->targetedUnit = NULL;
        }
    }

    unit->removedFromGame = true;


    //units.erase(std::remove(units.begin(), units.end(), unit), units.end());
    std::cout << "Implement removeUnit" << std::endl;

}

bool Player::checkDefeat(){
	int aliveUnits = 0;

    bool isDefeated = (units.size() > 0);
    defeated = isDefeated;
	if (defeated) {
		name_ += " [DEFEATED]";
	}
	

    return defeated;
}

bool Player::canPlace(std::shared_ptr<Unit> builder, std::shared_ptr<Unit> unit, Tile *_tile) {

    for (auto &tile : game_.map.getTiles(_tile, unit->width, unit->height)) {
        if(tile == builder->tile) // Ignore tile of the builder, this is handled in Unit::Build when builder despawns
            continue;

        if(!tile->isBuildable()) {
            return false;
        }

    }

    return true;
}

bool Player::canAfford(std::shared_ptr<Unit> unit) {
    return gold >= unit->goldCost and lumber >= unit->lumberCost and oil >= unit->oilCost;

}

void Player::addUnit(std::shared_ptr<Unit> u) {
    units.push_back(u);

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
    if(units.size() == 0){
        return -1;
    }
    if(!targetedUnit){
        return 0;
    }
    int idx = 0;
    for(auto &u : units){
        if(targetedUnit == u){
            return idx;
        }
        idx++;
    }
}

void Player::nextUnit(){
    int idx = _getNextPrevUnitIdx() + 1;
    if(idx == -1)
        return;
    idx++;
    targetedUnit = units[idx % units.size()];
}

void Player::previousUnit(){
    int idx = _getNextPrevUnitIdx();
    if(idx == -1)
        return;
    idx--;
    targetedUnit = units[idx % units.size()];

}

std::shared_ptr<Unit> Player::createUnit(int type_id) {
    switch (type_id){
        case Constants::Unit::Peasant:
            return std::shared_ptr<Unit>(new Peasant(*this));
        case Constants::Unit::TownHall:
            return std::shared_ptr<Unit>(new TownHall(*this));


    }
}
