//
// Created by Per-Arne on 26.02.2017.
//

#include "Building.h"
#include "../unit/unit.h"
#include "../util/Pathfinder.h"


void Building::update(std::shared_ptr<Unit> unit)const{

    unit->buildTimer += 1;
    if(unit->buildTimer >= unit->buildEntity->spawnDuration) {
        // Building is complete

        if(!unit->tile){
            // Unit has no tile, means unit is despawned
            Tile *firstWalkable = Pathfinder::find_first_walkable_tile(unit->buildEntity->tile);
            assert(firstWalkable);
            unit->setPosition(*firstWalkable);
            unit->transitionState();
        } else {
            // Unit has tile, needs to transition
            unit->transitionState();

        }


    }

}

void Building::end(std::shared_ptr<Unit> unit)const{

}

void Building::init(std::shared_ptr<Unit> unit)const{
    unit->buildTimer = 0;

}

