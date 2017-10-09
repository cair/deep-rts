//
// Created by Per-Arne on 27.02.2017.
//

#include "Dead.h"
#include "../unit/Unit.h"
#include "../Game.h"

void Dead::update(Unit & unit)const{

    unit.despawn();
    unit.player_->removeUnit(unit);
    unit.player_->checkDefeat();
    unit.player_->game_.checkTerminal();
}

void Dead::end(Unit & unit)const{

}

void Dead::init(Unit & unit)const{

}

