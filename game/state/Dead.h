//
// Created by Per-Arne on 25.02.2017.
//

#pragma once

#include "../Constants.h"
#include "BaseState.h"
class Unit;

class Dead: public BaseState {
public:
    Dead():BaseState(){
        id = Constants::State_Dead;
        name = "Dead";
    }

    virtual void update(Unit &unit)const;
    virtual void init(Unit &unit)const;
    virtual void end(Unit &unit)const;
};


