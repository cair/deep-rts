//
// Created by Per-Arne on 25.02.2017.
//

#pragma once

#include "../Constants.h"
#include "BaseState.h"
class Unit;

class Combat: public BaseState {
public:
    Combat():BaseState(){
        id = Constants::State_Combat;
        name = "Combat";
    }

    virtual void update(Unit &unit)const;
    virtual void init(Unit &unit)const;
    virtual void end(Unit &unit)const;
};

