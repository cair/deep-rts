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
    virtual void update(std::shared_ptr<Unit> unit)const;
    virtual void init(std::shared_ptr<Unit> unit)const;
    virtual void end(std::shared_ptr<Unit> unit)const;

};


#endif //WARC2SIM_IDLE_H
