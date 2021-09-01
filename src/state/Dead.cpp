//
// Created by Per-Arne on 27.02.2017.
//

#include "../../include/DeepRTS/state/Dead.h"
#include "../../include/DeepRTS/Unit.h"
#include "../../include/DeepRTS/Game.h"

void Dead::update(Unit & unit){
    unit.despawn();
    unit.player_.removeUnit(unit);
    unit.player_.playerState();
    unit.player_.getGame().isTerminal();
}

void Dead::end(Unit & unit){
    (void)(unit);
}

void Dead::init(Unit & unit){
    (void)(unit);
}

