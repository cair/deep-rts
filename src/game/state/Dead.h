//
// Created by Per-Arne on 25.02.2017.
//

#pragma once

#include "../Constants.h"
#include "BaseState.h"
class Unit;

class Dead: public BaseState {
public:
    Dead():BaseState(Constants::State_Dead){
        name = "Dead";
    }

    virtual void update(std::shared_ptr<Unit> unit)const;
    virtual void init(std::shared_ptr<Unit> unit)const;
    virtual void end(std::shared_ptr<Unit> unit)const;
};


