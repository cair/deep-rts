//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_WALKING_H
#define WARC2SIM_WALKING_H


#include "BaseState.h"

class Walking: public BaseState {

public:
    Walking(){
        id = Constants::State_Walking;
        name = "Walking";
    }
    virtual void update(Unit &unit)const;
    virtual void init(Unit &unit)const;
    virtual void end(Unit &unit)const;
};


#endif //WARC2SIM_WALKING_H
