//
// Created by Per-Arne on 24.02.2017.
//

#include "StateManager.h"

StateManager::StateManager()
{

}


BaseState *StateManager::getByID(int id){
    switch(id) {
        case Constants::State_Building:
            return &buildingState;
        case Constants::State_Spawning:
            return &spawnState;
        case Constants::State_Walking:
            return &walkingState;
        case Constants::State_Idle:
            return &idleState;
        case Constants::State_Despawned:
            return &despawnedState;
        case Constants::State_Harvesting:
            return &harvestingState;
        case Constants::State_Combat:
            return &combatState;
        case Constants::State_Dead:
            return &deadState;
    }
}
