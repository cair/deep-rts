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
    Despawned():BaseState(Constants::State::Despawned){
        name = "Despawned";
    }

    void update(Unit & unit) override;
    void init(Unit & unit) override;
    void end(Unit & unit) override;
};


#endif //WARC2SIM_DESPAWNED_H
