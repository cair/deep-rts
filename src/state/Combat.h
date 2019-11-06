//
// Created by Per-Arne on 25.02.2017.
//

#pragma once

#include "../Constants.h"
#include "BaseState.h"
class Unit;

class Combat: public BaseState {
public:
    Combat():BaseState(Constants::State::Combat){
        name = "Combat";
    }

    virtual void update(Unit & unit);
    virtual void init(Unit & unit);
    virtual void end(Unit & unit);
};

