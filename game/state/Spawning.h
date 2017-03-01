//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_SPAWNING_H
#define WARC2SIM_SPAWNING_H



#include "BaseState.h"

class Spawning: public BaseState{

public:
    Spawning():BaseState(){
        id = Constants::State_Spawning;
        name = "Spawning";
    }
    virtual void update(std::shared_ptr<Unit> unit)const;
    virtual void init(std::shared_ptr<Unit> unit)const;
    virtual void end(std::shared_ptr<Unit> unit)const;
};


#endif //WARC2SIM_SPAWNING_H
