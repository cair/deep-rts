//
// Created by Per-Arne on 24.02.2017.
//

#include "Walking.h"
#include "../player/Player.h"
#include "../Game.h"
#include "../util/Pathfinder.h"
#include "../util/JPS.h"

void Walking::update(Unit & unit)const{
    if(!unit.walkingGoal){
        assert(false); //No goal were set!
    }


    if (!unit.walking_path.empty()) {
        unit.walking_timer += 1;

        if (unit.walking_timer > unit.walking_interval) {

            // Pop next
            Tile * nextTile = unit.walking_path.back();
            unit.walking_path.pop_back();

			// Check if someone is standing on goal
			if (!nextTile->isWalkable()) {
				unit.transitionState();
				return;
			}


            unit.setPosition(*nextTile);
            unit.walking_timer = 0;


        }
    } else {
        unit.transitionState();
        return;
    }




}

void Walking::end(Unit & unit)const{
    unit.walking_path.clear();
}

void Walking::init(Unit & unit)const{
    unit.walking_timer = 0;

	Tile *goal = NULL;
	if (unit.walkingGoal->getOccupant()) {
		Unit* occupant = unit.walkingGoal->getOccupant();
		Tile *closest  = Pathfinder::find_closest_walkable_tile(unit.tile, occupant->tile, occupant->width);
		goal = closest;
	}
	else if (!unit.walkingGoal->isWalkable()) {
		goal = Pathfinder::find_first_walkable_tile(unit.walkingGoal);
	} else {
		goal = unit.walkingGoal;
	}

	if (!goal) {
		return;
	}
	unit.walkingGoal = goal;


	JPS::PathVector path; // The resulting path will go here.
	bool found = JPS::findPath(path, unit.player_->game_.map, 
		unit.tile->x, unit.tile->y,
		unit.walkingGoal->x, unit.walkingGoal->y, 1);

	unit.walking_path.clear();

	for(int i = path.size()-1; i >= 0; i--) {
		auto &pathItem = path[i];
		unit.walking_path.push_back(unit.player_->game_.map.getTile(pathItem.x, pathItem.y));
	}



}

