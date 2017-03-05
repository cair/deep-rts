//
// Created by Per-Arne on 24.02.2017.
//

#include "Walking.h"
#include "../player/Player.h"
#include "../Game.h"
#include "../util/Pathfinder.h"

void Walking::update(std::shared_ptr<Unit> unit)const{
    if(!unit->walkingGoal){
        assert(false); //No goal were set!
    }


    if (unit->walking_path.size() > 0)
    {

        unit->walking_timer += 1;
        if (unit->walking_timer > unit->walking_interval) {

            // Pop next
            Tile * nextTile = unit->walking_path.back();
            unit->walking_path.pop_back();
            unit->setPosition(*nextTile);
            unit->walking_timer = 0;

            /*
             *

    // Done
    if (unit->distance(*unit->walkingGoal) == 0) {
        unit->transitionState();
    return;
}



    // If someone is occupying tile
    if(nextTile->occupant){
        unit->walking_path = Pathfinder::aStar(unit->tile, unit->walkingGoal);
        return; // Try on next iteration // TODO?
    }



             */


        }
    } else {
        unit->transitionState();
        return;
    }




}

void Walking::end(std::shared_ptr<Unit> unit)const{
    unit->walking_path.clear();
}

void Walking::init(std::shared_ptr<Unit> unit)const{
    unit->walking_timer = 0;

	
	if (unit->walkingGoal->occupant) {
		std::shared_ptr<Unit> occupant = unit->walkingGoal->occupant;
		Tile *closest  = Pathfinder::find_closest_walkable_tile(unit->tile, occupant->tile, occupant->width);
		unit->walkingGoal = closest;
	}
	else if (!unit->walkingGoal->isWalkable()) {
		unit->walkingGoal = Pathfinder::find_first_walkable_tile(unit->walkingGoal);
	}

    unit->walking_path = Pathfinder::aStar(unit->tile, unit->walkingGoal);
}

