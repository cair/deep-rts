//
// Created by Per-Arne on 01.03.2017.
//

#include "Algorithm.h"
#include "../../player/Player.h"

void Algorithm::setPlayerName(std::string name){
    player.setName(name);
}

std::shared_ptr<BaseAction> Algorithm::getAction(int actionID, std::shared_ptr<Unit> unitptr) {

    switch (actionID){
		case Constants::Action::NextUnit:
            unitptr->player_.nextUnit();
            break;
		case Constants::Action::PreviousUnit:
            unitptr->player_.previousUnit();
            break;
		case Constants::Action::RightUpRight:
            unitptr->rightClickRelative(-1, 1);
            break;
		case Constants::Action::RightUpLeft:
            unitptr->rightClickRelative(-1, -1);
            break;
		case Constants::Action::RightDownRight:
            unitptr->rightClickRelative(1, 1);
            break;
		case Constants::Action::RightDownLeft:
            unitptr->rightClickRelative(1, -1);
            break;
		case Constants::Action::RightUp:
            unitptr->rightClickRelative(0, -1);
            break;
		case Constants::Action::RightDown:
            unitptr->rightClickRelative(0, 1);
            break;
		case Constants::Action::RightLeft:
            unitptr->rightClickRelative(-1, 0);
            break;
		case Constants::Action::RightRight:
            unitptr->rightClickRelative(1, 0);
            break;
		case Constants::Action::Build0:
			unitptr->build(0);
			break;
		case Constants::Action::Build1:
			unitptr->build(1);
			break;
		case Constants::Action::Build2:
			unitptr->build(2);
			break;

    }

    return std::shared_ptr<BaseAction>();
}
