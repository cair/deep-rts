//
// Created by Per-Arne on 26.02.2017.
//

#include "Building.h"
#include "../unit/Unit.h"
#include "../util/Pathfinder.h"


void Building::update(Unit & unit)const{
    Unit &buildEntity = unit.getBuildEntity();

    if(unit.buildTimer >= buildEntity.spawnDuration) {
        // Building is complete

        if(!unit.tile){
            // Unit has no tile, means unit is despawned
            Tile *firstWalkable = Pathfinder::find_first_walkable_tile(buildEntity.tile);
            assert(firstWalkable);
            unit.setPosition(*firstWalkable);
            unit.transitionState();
        } else {
			
			// Unit has tile, needs to transition
            unit.transitionState();

        }


    }

}

void Building::end(Unit & unit)const{
	unit.buildEntityID = -1;
}

void Building::init(Unit & unit)const{
    unit.buildTimer = 0;

}

