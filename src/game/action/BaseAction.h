//
// Created by Per-Arne on 01.03.2017.
//

#ifndef WARC2SIM_BASEACTION_H
#define WARC2SIM_BASEACTION_H


#include "../unit/Unit.h"

class BaseAction {
protected:
    Unit & unit;
public:
    BaseAction(Unit & unit): unit(unit){};
    long tick;                      // At which tick this move was executed;
    virtual void doAction() = 0;         // This function
    virtual void reverseAction() = 0;    // Reverse operation of this action.
};


#endif //WARC2SIM_BASEACTION_H
