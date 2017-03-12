//
// Created by Per-Arne on 01.03.2017.
//

#include "AlgoRandom.h"
#include "../../player/Player.h"
#include "../../Game.h"

AlgoRandom::AlgoRandom(Player &player) : Algorithm(player), rgen(rd()) {

	player.name_ += " [RND]";

    defineActionSpace();
    if(!player.targetedUnit) {
        if(player.unitIndexes.size() == 0) {
            return;
        }

        // Select first available unit
        player.targetedUnit = &player.game_.getUnit(player.unitIndexes[0]);
    }
}

void AlgoRandom::update() {
    // Tick happended

	std::uniform_int_distribution<int> dist(0, actionSpace.size() - 1);
	

	int randomIndex = dist(rgen);
	Constants::Action actionID = actionSpace[randomIndex];

	player.queueAction(actionID);
	/*int nUnits = player.unitIndexes.size();
	for (int i = 0; i < nUnits; i++) {
		Unit & u = player.game_.getUnit(player.unitIndexes[i]);
		if (u.state->id == Constants::State::Idle) {
			
			int actionID = actionSpace[randomIndex];
		
			
			//std::shared_ptr<BaseAction> action = findBestAction(u);
			//doAction(action);
		}

    
    }*/

}

void AlgoRandom::terminal() {

}

std::shared_ptr<BaseAction> AlgoRandom::findBestAction(Unit & unit) {
    int randomIndex = rand() % actionSpace.size();
    int actionID = actionSpace[randomIndex];
    std::shared_ptr<BaseAction> selectedAction = getAction(actionID, &unit);
    assert(selectedAction);
    return selectedAction;
}

void AlgoRandom::doAction(std::shared_ptr<BaseAction> action) {
    action->doAction();
}

void AlgoRandom::defineActionSpace() {

    actionSpace = {
		Constants::Action::PreviousUnit,
		Constants::Action::NextUnit,
		Constants::Action::RightLeft,
		Constants::Action::RightRight,
		Constants::Action::RightUp,
		Constants::Action::RightDown,
		Constants::Action::RightUpLeft,
		Constants::Action::RightUpRight,
		Constants::Action::RightDownLeft,
		Constants::Action::RightDownRight,
		Constants::Action::Build0,
		Constants::Action::Build1,
		Constants::Action::Build2,
		Constants::Action::NoAction
    };
}
