//
// Created by Per-Arne on 01.03.2017.
//

#ifndef WARC2SIM_RIGHTCLICKACTION_H
#define WARC2SIM_RIGHTCLICKACTION_H


#include "BaseAction.h"

class RightClickAction: public BaseAction {
    int clickX;
    int clickY;
public:
    RightClickAction(Unit & unit);
    virtual void doAction();
    virtual void reverseAction();

};


#endif //WARC2SIM_RIGHTCLICKACTION_H
