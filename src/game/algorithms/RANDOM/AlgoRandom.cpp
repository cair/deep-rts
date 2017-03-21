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

	actionInterval = 1000 / 50;
	nextAction = clock();
}

void AlgoRandom::update() {
    // Tick happended
	now = clock();
	if (now > nextAction) {
		std::uniform_int_distribution<int> dist(0, actionSpace.size() - 1);
		int randomIndex = dist(rgen);
		std::cout << randomIndex << std::endl;
		Constants::Action actionID = actionSpace[randomIndex];
		player.queueAction(actionID);
		nextAction = clock() + actionInterval;
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
