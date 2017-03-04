//
// Created by Per-Arne on 24.02.2017.
//


#include "Spawning.h"
#include "../unit/unit.h"

void Spawning::update(std::shared_ptr<Unit> unit)const{
    unit->spawnTimer += 1;

   
    if(unit->spawnTimer >= unit->spawnDuration) {
        // Unit can spawn
        unit->setPosition(*unit->spawnTile);
        unit->transitionState();
    }

}

void Spawning::init(std::shared_ptr<Unit> unit)const{

}

void Spawning::end(std::shared_ptr<Unit> unit)const{

}
