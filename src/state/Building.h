//
// Created by Per-Arne on 25.02.2017.
//

#pragma once

#include "../Constants.h"
#include "BaseState.h"
class Unit;

class Building: public BaseState {
public:
    Building():BaseState(Constants::State::Building){
        name = "Building";
    }

    void update(Unit & unit) override;
    void init(Unit & unit) override;
    void end(Unit & unit) override;
};

