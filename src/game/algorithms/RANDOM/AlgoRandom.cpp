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
		if (u->state->id == Constants::State_Idle) {
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
            Constants::ACTION_NEXT_UNIT,
            Constants::ACTION_PREVIOUS_UNIT,
            Constants::ACTION_RC_DOWN,
            Constants::ACTION_RC_LEFT,
            Constants::ACTION_RC_RIGHT,
            Constants::ACTION_RC_UP,
            Constants::ACTION_RC_DOWNLEFT,
            Constants::ACTION_RC_DOWNRIGHT,
            Constants::ACTION_RC_UPLEFT,
            Constants::ACTION_RC_UPRIGHT,
			Constants::ACTION_BUILD_0,
			Constants::ACTION_BUILD_1,
			Constants::ACTION_BUILD_2

    };
}
