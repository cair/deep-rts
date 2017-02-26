//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_STATEMANAGER_H
#define WARC2SIM_STATEMANAGER_H


#include "Spawning.h"
#include "Walking.h"
#include "Idle.h"
#include "Despawned.h"

class StateManager {

public:
    StateManager();
    Walking walkingState;
    Spawning spawnState;
    Idle idleState;
    Despawned despawnedState;
};


#endif //WARC2SIM_STATEMANAGER_H
