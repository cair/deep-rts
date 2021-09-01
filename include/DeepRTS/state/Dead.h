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

    void update(Unit & unit) override;
    void init(Unit & unit) override;
    void end(Unit & unit) override;
};


