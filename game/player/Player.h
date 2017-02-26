//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_PLAYER_H
#define WARC2SIM_PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include "../unit/Unit.h"
#include "../environment/Tile.h"


class Game;
class Player {
private:
    static int gId;

    std::string name = "Derp";
    int oil;
    int gold;
    int lumber;
    int foodConsumption;
    int food;
    int faction; // 0 = Human, 1 = Orc




public:
    Game &game_;
    std::vector<Unit> units;

    void update();

    int getId();
    int getFoodConsumption();
    int getFood();
    int getGold();
    int getOil();
    int getLumber();
    int getUnitCount();

    Player(Game &game);
    Unit &spawn(Tile &spawnPoint);


    int id_;
};


#endif //WARC2SIM_PLAYER_H
