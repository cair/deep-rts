//
// Created by Per-Arne on 24.02.2017.
//


#include "Player.h"
#include "../unit/Peasant.h"
#include "../Game.h"
#include "../lib/ColorConverter.h"
#include "../algorithms/RANDOM/AlgoRandom.h"

int Player::gId = 0;

Player::Player(Game &game): inventoryManager(*this), game_(game) {
    id_ = Player::gId++;
    name_ = "Player: " + std::to_string(id_);
    playerColor = ColorConverter::hsv(fmod(id_ * 0.618033988749895, 1.0),
                                      0.5,
                                      sqrt(1.0 - fmod(id_ * 0.618033988749895, 0.5)));


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

int Player::getUnitCount() {
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
    int GOLD_VALUE = 2;
    int LUMBER_VALUE = 1;
    int OIL_VALUE = 3;


    int gatherScore = (statGoldGather * GOLD_VALUE + statLumberGather * LUMBER_VALUE) * .20;
    int builtScore = statUnitBuilt * 10;
    int damageScore = statUnitDamageDone / (statUnitDamageTaken + 1);
    int unitScore = units.size() * 2.5;
    int militaryScore = 0;
    int defenceScore = 0;


    return gatherScore + builtScore + unitScore + militaryScore + defenceScore + damageScore;

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
    bool isDefeated = (units.size() > 0);
    defeated = isDefeated;
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

void Player::setAlgorithm(std::shared_ptr<AlgoRandom> theAlg){
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