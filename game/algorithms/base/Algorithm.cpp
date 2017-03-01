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
        case Constants::ACTION_NEXT_UNIT:
            unitptr->player_.nextUnit();
            break;
        case Constants::ACTION_PREVIOUS_UNIT:
            unitptr->player_.previousUnit();
            break;
        case Constants::ACTION_RC_UPRIGHT:
            unitptr->moveRelative(-1, 1);
            break;
        case Constants::ACTION_RC_UPLEFT:
            unitptr->moveRelative(-1, -1);
            break;
        case Constants::ACTION_RC_DOWNRIGHT:
            unitptr->moveRelative(1, 1);
            break;
        case Constants::ACTION_RC_DOWNLEFT:
            unitptr->moveRelative(1, -1);
            break;
        case Constants::ACTION_RC_UP:
            unitptr->moveRelative(0, -1);
            break;
        case Constants::ACTION_RC_DOWN:
            unitptr->moveRelative(0, 1);
            break;
        case Constants::ACTION_RC_LEFT:
            unitptr->moveRelative(-1, 0);
            break;
        case Constants::ACTION_RC_RIGHT:
            unitptr->moveRelative(1, 0);
            break;

    }

    return std::shared_ptr<BaseAction>();
}
