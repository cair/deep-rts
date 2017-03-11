//
// Created by Per-Arne on 01.03.2017.
//

#include "AlgoRandom.h"
#include "../../player/Player.h"

AlgoRandom::AlgoRandom(Player &player) : Algorithm(player) {

	player.name_ += " [AlgoRandom]";

    defineActionSpace();
    if(!player.targetedUnit) {
        if(player.units.size() == 0) {
            // Defeated, cannot select...
            return;
        }

        // Select first available unit
        player.targetedUnit = player.units[0];
    }
}

void AlgoRandom::update() {
    // Tick happended

	int nUnits = player.units.size();


	for (int i = 0; i < nUnits; i++) {
		std::shared_ptr<Unit> u = player.units[i];
		if (u->state->id == Constants::State::Idle) {
			int randomIndex = rand() % actionSpace.size();
			int actionID = actionSpace[randomIndex];
			getAction(actionID, u);
			//std::shared_ptr<BaseAction> action = findBestAction(u);
			//doAction(action);
		}

    
    }

}

void AlgoRandom::terminal() {

}

std::shared_ptr<BaseAction> AlgoRandom::findBestAction(std::shared_ptr<Unit> unit) {
    int randomIndex = rand() % actionSpace.size();
    int actionID = actionSpace[randomIndex];
    std::shared_ptr<BaseAction> selectedAction = getAction(actionID, unit);
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
