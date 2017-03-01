//
// Created by Per-Arne on 27.02.2017.
//

#include "Dead.h"
#include "../unit/Unit.h"
#include "../player/Player.h"
void Dead::update(std::shared_ptr<Unit> unit)const{

    unit->despawn();
    unit->player_.removeUnit(unit);
}

void Dead::end(std::shared_ptr<Unit> unit)const{

}

void Dead::init(std::shared_ptr<Unit> unit)const{

}

