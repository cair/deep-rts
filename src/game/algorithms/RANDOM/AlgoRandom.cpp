//
// Created by Per-Arne on 01.03.2017.
//

#include "AlgoRandom.h"
#include "../../player/Player.h"
#include "../../Game.h"

AlgoRandom::AlgoRandom(Player *player) : Algorithm(player), rgen(rd()) {

	player->name_ += " [RND]";

    defineActionSpace();
    if(!player->targetedUnit) {
        if(player->unitIndexes.size() == 0) {
            return;
        }

        // Select first available unit
        player->targetedUnit = &player->game_.getUnit(player->unitIndexes[0]);
    }
}

void AlgoRandom::update() {

	// AI Find best action
	int actionID = findAction();

	// Translate AI action to Generic action
	Constants::Action actionC = getAction(actionID);

	// Queue Action
	player->queueAction(actionC);
}


int AlgoRandom::findAction() {
	std::uniform_int_distribution<int> dist(0, actionSpace.size() - 1);
	int randomIndex = dist(rgen);
    return randomIndex;
}

void AlgoRandom::defineActionSpace() {

    actionSpace = {
		Constants::Action::PreviousUnit,
		Constants::Action::NextUnit,

		Constants::Action::MoveLeft,
		Constants::Action::MoveRight,
		Constants::Action::MoveUp,
		Constants::Action::MoveDown,
		Constants::Action::MoveUpLeft,
		Constants::Action::MoveUpRight,
		Constants::Action::MoveDownLeft,
		Constants::Action::MoveDownRight,

		Constants::Action::Attack,
		Constants::Action::Harvest,

		Constants::Action::Build0,
		Constants::Action::Build1,
		Constants::Action::Build2,
		Constants::Action::NoAction
    };
}
