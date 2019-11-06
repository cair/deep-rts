//
// Created by Per-Arne on 26.02.2017.
//

#ifndef WARC2SIM_HARVESTING_H
#define WARC2SIM_HARVESTING_H


#include "BaseState.h"

class Unit;

class Harvesting: public BaseState {
public:
    Harvesting():BaseState(Constants::State::Harvesting){
        name = "Harvesting";
    }

    void update(Unit & unit) override;
    void init(Unit & unit) override;
    void end(Unit & unit) override;
};

#endif //WARC2SIM_HARVESTING_H
