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
#include "../algorithms/base/Algorithm.h"
//#include "../util/SharedQueue.h"
#include <deque>

class AlgoRandom;


class Game;
class Player {
private:

    std::string name = "[NO NAME]";
	std::deque<Constants::Action> actionQueue;




public:
    int oil;
    int gold;
    int lumber;
    int foodConsumption;
    int food;
    int faction; // 0 = Human, 1 = Orc

	int apm_counter = 0; // Incremental APM counter which is used as a sampler
	int apm = 0; // Current average APM

    Game &game_;

    std::tuple<uint8_t , uint8_t, uint8_t> playerColor;
    int targetedUnitID = -1;

	int id_;
	bool defeated;
	std::string name_;
	Algorithm *algorithm_ = NULL;
	int statGoldGather;
	int statLumberGather;
	int statOilGather;
	int statUnitDamageDone;
	int statUnitDamageTaken;
	int statUnitBuilt;
	int statUnitMilitary;
	int getScore();
	std::vector<uint16_t> unitIndexes; // Vector of all of player's units (Where they are located in vector)


    void update();
	void reset();

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
    Unit &addUnit(Constants::Unit unitType);
	void queueAction(Constants::Action actionID);
	void queueAction(int actionID);
	int actionStatistics[20];
	size_t getQueueSize();

    Player(Game &game, int id);

	Unit *getTargetedUnit();



    Unit &spawn(Tile &spawnPoint);


 

    bool canPlace(Unit & builder, Unit & unit, Tile *tile);

    bool canAfford(Unit & unit);

    void removeUnit(Unit & unit);

    bool checkDefeat();
    void setName(std::string name);



    void setAlgorithm(Algorithm *theAlg);



    void nextUnit();

    void previousUnit();

    int _getNextPrevUnitIdx();


    void rightClick(Position pos);
};


#endif //WARC2SIM_PLAYER_H
