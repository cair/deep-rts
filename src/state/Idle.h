//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_IDLE_H
#define WARC2SIM_IDLE_H


#include "BaseState.h"

class Idle: public BaseState {

public:
    Idle():BaseState(Constants::State::Idle){
        name = "Idle";
    }
    void update(Unit & unit) override;
    void init(Unit & unit) override;
    void end(Unit & unit) override;

};


#endif //WARC2SIM_IDLE_H
