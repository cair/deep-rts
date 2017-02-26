//
// Created by Per-Arne on 24.02.2017.
//


#include "Player.h"
#include "../unit/Peasant.h"
#include "../Game.h"

int Player::gId = 0;

Player::Player(Game &game): game_(game) {
    id_ = Player::gId++;
    faction = 0;
    gold = 1500;
    lumber = 750;
    oil = 0;
    foodConsumption = 0;
    food = 1;

}


Unit& Player::spawn(Tile &spawnPoint) {
    // Spawn a builder

    Config::print("Spawning player_!");


    if(faction == 0) // Human
    {
        units.push_back(Peasant(*this));
    }
    else if(faction == 1) // Orc
    {
        units.push_back(Peasant(*this));
    }
    else {
        assert(false);
    }

    assert(!units.empty());
    Unit &unit = units.back();
    unit.spawn(spawnPoint);
    unit.spawnTimer = unit.spawnDuration; // Spawn instantly


    return unit;
}

void Player::update() {

    for(auto &unit : units) {
        unit.update();

        int r = ((double) rand() / (RAND_MAX)) + 1;
        if (r > 0.75) {

            int rnd = rand() % (game_.map.tiles.size() - 1);
            std::cout << rnd << " --- " << game_.map.tiles.size() << std::endl;
            Tile &randomTile = game_.map.tiles[rnd];
            unit.rightClick(randomTile);
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

int Player::getUnitCount() {
    return units.size();
}

int Player::getId() {
    return id_;
}

