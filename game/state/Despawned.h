//
// Created by Per-Arne on 25.02.2017.
//

#ifndef WARC2SIM_DESPAWNED_H
#define WARC2SIM_DESPAWNED_H


#include "../Constants.h"
#include "BaseState.h"
class Unit;

class Despawned: public BaseState {
public:
    Despawned():BaseState(Constants::State_Despawned){
        name = "Despawned";
    }

    virtual void update(std::shared_ptr<Unit> unit)const;
    virtual void init(std::shared_ptr<Unit> unit)const;
    virtual void end(std::shared_ptr<Unit> unit)const;
};


#endif //WARC2SIM_DESPAWNED_H
