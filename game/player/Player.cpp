//
// Created by Per-Arne on 24.02.2017.
//


#include "Player.h"
#include "../unit/Peasant.h"
#include "../Game.h"
#include "../lib/ColorConverter.h"

int Player::gId = 0;

Player::Player(Game &game): inventoryManager(*this), game_(game) {
    id_ = Player::gId++;


    /*playerColor = ColorConverter::hsv(fmod(id_ * 0.618033988749895, 1.0),
                                      0.5,
                                      sqrt(1.0 - fmod(id_ * 0.618033988749895, 0.5)));
    std::cout << std::to_string(id_) << "--" << std::to_string(playerColor.r) << "," << std::to_string(playerColor.g) << "," << std::to_string(playerColor.b) << std::endl;
    */

    faction = 0;
    gold = 1500;
    lumber = 750;
    oil = 0;
    foodConsumption = 0;
    food = 1;

}


Unit& Player::spawn(Tile &spawnPoint) {
    // Spawn a builder



    if(faction == 0) // Human
    {
        units.push_back(new Peasant(*this));
    }
    else if(faction == 1) // Orc
    {
        units.push_back(new Peasant(*this));
    }
    else {
        assert(false);
    }

    assert(!units.empty());
    Unit *unit = units.back();
    unit->spawn(spawnPoint, unit->spawnDuration);



    return *unit;
}

void Player::update() {

    for(auto &unit : units) {
        unit->update();
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

void Player::removeUnit(Unit *unit) {
    std::cout << "Implement removeUnit" << std::endl;
    /*auto it = std::find_if(units.begin(), units.end(), [=](Unit* p)
                      {
                          return unit->id == p->id;
                      });

    if(it != units.end())
    {
        //object found
    }*/
}

bool Player::checkDefeat(){
    bool isDefeated = (units.size() > 0);
    defeated = isDefeated;
    return defeated;
}

bool Player::canPlace(Unit *unit, Tile *_tile) {

    for (auto &tile : game_.map.getTiles(_tile, unit->width, unit->height)) {
        if(tile == _tile)
            continue;

        if(!tile->isBuildable()) {
            return false;
        }

    }

    return true;
}

bool Player::canAfford(Unit *unit) {
    return gold >= unit->goldCost and lumber >= unit->lumberCost and oil >= unit->oilCost;

}

void Player::addUnit(Unit *u) {
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
