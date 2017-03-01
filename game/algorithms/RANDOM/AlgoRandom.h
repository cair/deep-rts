//
// Created by Per-Arne on 01.03.2017.
//

#ifndef WARC2SIM_ALGORANDOM_H
#define WARC2SIM_ALGORANDOM_H


#include "../base/Algorithm.h"

class AlgoRandom: public Algorithm{
public:
    AlgoRandom(Player &player);
    virtual void update();
    virtual void terminal();
    virtual std::shared_ptr<BaseAction> findBestAction(std::shared_ptr<Unit> unit);
    virtual void doAction(std::shared_ptr<BaseAction> action);
    virtual void defineActionSpace();


};


#endif //WARC2SIM_ALGORANDOM_H
