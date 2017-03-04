//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_STATEMANAGER_H
#define WARC2SIM_STATEMANAGER_H


#include "Spawning.h"
#include "Walking.h"
#include "Idle.h"
#include "Despawned.h"
#include "Harvesting.h"
#include "Building.h"
#include "Combat.h"
#include "Dead.h"

class StateManager {

public:
    StateManager();
    Walking walkingState;
    Spawning spawnState;
    Idle idleState;
    Despawned despawnedState;
    Harvesting harvestingState;
    Building buildingState;
    Combat combatState;
    Dead deadState;

    BaseState *getByID(int id);
};


#endif //WARC2SIM_STATEMANAGER_H
