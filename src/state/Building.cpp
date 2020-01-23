//
// Created by Per-Arne on 26.02.2017.
//

#include "Building.h"
#include "../unit/Unit.h"
#include "../util/Pathfinder.h"
#include "../player/Player.h"


void Building::update(Unit & unit){
    Unit &buildEntity = unit.getBuildEntity();

    unit.buildTimer += 1;
    if(unit.buildTimer >= buildEntity.spawnDuration) {
        // Building is complete

        if(!unit.tile){
            // Unit has no tile, means unit is despawned
            Tile *firstWalkable = Pathfinder::find_first_walkable_tile(buildEntity.tile);
            //assert(firstWalkable);
            unit.setPosition(*firstWalkable);
            unit.transitionState();
            unit.player_.food += unit.foodProduction;
            unit.player_.foodConsumption += unit.foodConsumption;
        } else {
			
			// Unit has tile, needs to transition
            unit.transitionState();

        }


    }

}

void Building::end(Unit & unit){
	unit.buildEntityID = -1;
    unit.buildTimer = 0;
}

void Building::init(Unit & unit){

    // Build timer is reset on end. this is so that we can override build time (i.e on spawn)
}

