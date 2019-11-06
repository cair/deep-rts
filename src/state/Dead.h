//
// Created by Per-Arne on 25.02.2017.
//

#pragma once

#include "../Constants.h"
#include "BaseState.h"
class Unit;

class Dead: public BaseState {
public:
    Dead():BaseState(Constants::State::Dead){
        name = "Dead";
    }

    virtual void update(Unit & unit);
    virtual void init(Unit & unit);
    virtual void end(Unit & unit);
};


