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
#include "../unit/InventoryManager.h"


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
    InventoryManager inventoryManager;
    std::vector<Unit*> units;
    sf::Color playerColor;

    void update();

    int getId();
    int getFoodConsumption();
    int getFood();
    int getGold();
    int getOil();
    int getLumber();
    int getUnitCount();
    void subGold(int n);
    void subLumber(int n);
    void subOil(int n);
    void addGold(int n);
    void addLumber(int n);
    void addOil(int n);
    void addUnit(Unit *u);
    Player(Game &game);
    Unit &spawn(Tile &spawnPoint);


    int id_;

    bool canPlace(Unit *unit, Tile *tile);

    bool canAfford(Unit *unit);

    void removeUnit(Unit *unit);

    bool checkDefeat();

    bool defeated;
};


#endif //WARC2SIM_PLAYER_H
