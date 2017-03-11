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
#include "../algorithms/base/Algorithm.h"


class AlgoRandom;


class Game;
class Player {
private:
    static int gId;

    std::string name = "Derp";





public:
    int oil;
    int gold;
    int lumber;
    int foodConsumption;
    int food;
    int faction; // 0 = Human, 1 = Orc

    Game &game_;
    InventoryManager inventoryManager;

    sf::Color playerColor;
    Unit *targetedUnit = NULL;

    void update();

    int getId();
    int getFoodConsumption();
    int getFood();
    int getGold();
    int getOil();
    int getLumber();
	size_t getUnitCount();
    void subGold(int n);
    void subLumber(int n);
    void subOil(int n);
    void addGold(int n);
    void addLumber(int n);
    void addOil(int n);
    Unit &addUnit(Unit& u);
    Unit createUnit(int type_id);

    Player(Game &game);
    Unit &spawn(Tile &spawnPoint);


    int id_;

    bool canPlace(Unit & builder, Unit & unit, Tile *tile);

    bool canAfford(Unit & unit);

    void removeUnit(Unit & unit);

    bool checkDefeat();

    bool defeated;

    void setName(std::string name);

    std::string name_;

    void setAlgorithm(std::shared_ptr<Algorithm> theAlg);

    std::shared_ptr<Algorithm> algorithm_ = NULL;

    void nextUnit();

    void previousUnit();

    int _getNextPrevUnitIdx();

    int statGoldGather;
    int statLumberGather;
    int statOilGather;
    int statUnitDamageDone;
    int statUnitDamageTaken;
    int statUnitBuilt;
    int statUnitMilitary;

    int getScore();
};


#endif //WARC2SIM_PLAYER_H
