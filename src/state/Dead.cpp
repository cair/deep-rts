//
// Created by Per-Arne on 27.02.2017.
//

#include "Dead.h"
#include "../unit/Unit.h"
#include "../Game.h"

void Dead::update(Unit & unit){
    unit.despawn();
    unit.player_.removeUnit(unit);
    unit.player_.isDefeated();
    unit.player_.getGame().isTerminal();
}

void Dead::end(Unit & unit){
    (void)(unit);
}

void Dead::init(Unit & unit){
    (void)(unit);
}

