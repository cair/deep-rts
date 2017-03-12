//
// Created by Per-Arne on 01.03.2017.
//

#ifndef WARC2SIM_ALGORITHM_H
#define WARC2SIM_ALGORITHM_H

#include <memory>
#include "../../unit/Unit.h"
#include "../../action/BaseAction.h"

class Player;
class Action;
class Algorithm {

protected:
    Player &player;
    std::vector<Constants::Action> actionSpace;

public:
    Algorithm(Player &player): player(player){
        defineActionSpace();
    };
    virtual void update() = 0;
    virtual void terminal() = 0;
    virtual void defineActionSpace(){};
    virtual std::shared_ptr<BaseAction> findBestAction(Unit & unit) = 0;
    virtual void doAction(std::shared_ptr<BaseAction> action) = 0;
    void setPlayerName(std::string name);
    std::shared_ptr<BaseAction> getAction(int actionID, Unit *unitptr);
};


#endif //WARC2SIM_ALGORITHM_H
