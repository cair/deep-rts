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

    void update(Unit & unit) override;
    void init(Unit & unit) override;
    void end(Unit & unit) override;
};

