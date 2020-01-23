//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_SPAWNING_H
#define WARC2SIM_SPAWNING_H



#include "BaseState.h"

class Spawning: public BaseState{

public:
    Spawning():BaseState(Constants::State::Spawning){
        name = "Spawning";
    }
    void update(Unit & unit) override;
    void init(Unit & unit) override;
    void end(Unit & unit) override;
};


#endif //WARC2SIM_SPAWNING_H
