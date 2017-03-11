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


    if (!unit->walking_path.empty()) {
        unit->walking_timer += 1;

        if (unit->walking_timer > unit->walking_interval) {

            // Pop next
            Tile * nextTile = unit->walking_path.back();
            unit->walking_path.pop_back();

			// Check if someone is standing on goal
			if (!nextTile->canWalkTo()) {
				unit->transitionState();
				return;
			}


            unit->setPosition(*nextTile);
            unit->walking_timer = 0;


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

	Tile *goal = NULL;
	if (unit->walkingGoal->getOccupant()) {
		std::shared_ptr<Unit> occupant = unit->walkingGoal->getOccupant();
		Tile *closest  = Pathfinder::find_closest_walkable_tile(unit->tile, occupant->tile, occupant->width);
		goal = closest;
	}
	else if (!unit->walkingGoal->canWalkTo()) {
		goal = Pathfinder::find_first_walkable_tile(unit->walkingGoal);
	}
	else {
		goal = unit->walkingGoal;
	}


	if (!goal) {

		return;
	}

	unit->walkingGoal = goal;
	Pathfinder::aStar(unit->walking_path, unit->tile, goal);
}

