//
// Created by Per-Arne on 01.03.2017.
//

#include "RightClickAction.h"

RightClickAction::RightClickAction(Unit & unit) : BaseAction(unit), clickX(0), clickY(0) {

    clickY = 1;
    clickX = 1;
}

void RightClickAction::doAction() {

}

void RightClickAction::reverseAction() {

}
