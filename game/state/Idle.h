//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_IDLE_H
#define WARC2SIM_IDLE_H


#include "BaseState.h"

class Idle: public BaseState {

public:
    Idle():BaseState(){
        id = Constants::State_Idle;
        name = "Idle";
    }
    virtual void update(Unit &unit)const;
    virtual void init(Unit &unit)const;
    virtual void end(Unit &unit)const;

};


#endif //WARC2SIM_IDLE_H
