//
// Created by Per-Arne on 01.03.2017.
//

#include "AlgoRandom.h"
#include "../../player/Player.h"
#include "../../Game.h"

AlgoRandom::AlgoRandom(Player &player) : Algorithm(player) {

	player.name_ += " [AlgoRandom]";

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

	int nUnits = player.unitIndexes.size();
	for (int i = 0; i < nUnits; i++) {
		Unit & u = player.game_.getUnit(player.unitIndexes[i]);
		if (u.state->id == Constants::State::Idle) {
			int randomIndex = rand() % actionSpace.size();
			int actionID = actionSpace[randomIndex];
			getAction(actionID, &u);
			//std::shared_ptr<BaseAction> action = findBestAction(u);
			//doAction(action);
		}

    
    }

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
		Constants::Action::NextUnit,
		Constants::Action::PreviousUnit,
		Constants::Action::RightUpRight,
		Constants::Action::RightUpLeft,
		Constants::Action::RightDownRight,
		Constants::Action::RightDownLeft,
		Constants::Action::RightUp,
		Constants::Action::RightDown,
		Constants::Action::RightLeft,
		Constants::Action::RightRight,
		Constants::Action::Build0,
		Constants::Action::Build1,
		Constants::Action::Build2
    };
}
