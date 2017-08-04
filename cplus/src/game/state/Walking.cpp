//
// Created by Per-Arne on 24.02.2017.
//

#include "Walking.h"
#include "../player/Player.h"
#include "../Game.h"
#include "../util/Pathfinder.h"
#include "../util/JPS.h"

void Walking::update(Unit & unit)const{
	Tile *walkingGoal = unit.getTile(unit.walkingGoalID);


    if(!walkingGoal){
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

            if(unit.stepsLeft == 1) {
                unit.transitionState();
            }
            if(unit.stepsLeft >= 1) {
                unit.stepsLeft --;
            }


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

    // Retrieve Tile* from walkingGoalID
	Tile *walkingGoal = unit.getTile(unit.walkingGoalID);

    // Predefine goal pointer
	Tile *goal = NULL;

    // Check if walking goal has a occupant
    if (walkingGoal->getOccupantID() != Constants::None) {
        // Retrieve the occupant
		Unit* occupant = walkingGoal->getOccupant();

        // Since there is a occupant on the walking goal, a new alternative neighbor must be found
        goal  = Pathfinder::find_closest_walkable_tile(unit.tile, occupant->tile, occupant->width);
	}
	else if (!walkingGoal->isWalkable()) {
        // Goal is not walkable, and we must find closest alternative
		goal = Pathfinder::find_first_walkable_tile(walkingGoal);
	} else {
        // Goal is walkable and there is no occupants there
		goal = walkingGoal;
	}

    // No goal has been set
	if (!goal) {
		return;
	}

    // Clear existing walking path
    unit.walking_path.clear();

    // Set walkingGoalID to the set Goal
	unit.walkingGoalID = goal->id;

    // If the distance is only 1 n length, there is no need to calculate path
    if(unit.tile->distance(goal) == 1) {
        unit.walking_path.push_back(goal);
        return;
    }


    // The resulting path will go here.
    JPS::PathVector path;

    // Attempt to find a viable path
    JPS::findPath(
                path,
                unit.player_->game_.map,
                unit.tile->x, unit.tile->y,
                goal->x,
                goal->y,
                1);



    // Insert found path to the walking path vector
    for(int i = path.size()-1; i >= 0; i--) {
            auto &pathItem = path[i];
            unit.walking_path.push_back(unit.player_->game_.map.getTile(pathItem.x, pathItem.y));
    }
}

