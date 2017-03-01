//
// Created by Per-Arne on 26.02.2017.
//

#ifndef WARC2SIM_HARVESTING_H
#define WARC2SIM_HARVESTING_H


#include "BaseState.h"

class Unit;

class Harvesting: public BaseState {
public:
    Harvesting():BaseState(){
        id = Constants::State_Harvesting;
        name = "Harvesting";
    }

    virtual void update(std::shared_ptr<Unit> unit)const;
    virtual void init(std::shared_ptr<Unit> unit)const;
    virtual void end(std::shared_ptr<Unit> unit)const;
};

#endif //WARC2SIM_HARVESTING_H
