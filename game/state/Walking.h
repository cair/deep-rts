//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_WALKING_H
#define WARC2SIM_WALKING_H


#include "BaseState.h"

class Walking: public BaseState {

public:
    Walking(): BaseState(Constants::State_Walking){
        name = "Walking";
    }
    virtual void update(std::shared_ptr<Unit> unit)const;
    virtual void init(std::shared_ptr<Unit> unit)const;
    virtual void end(std::shared_ptr<Unit> unit)const;
};


#endif //WARC2SIM_WALKING_H
