//
// Created by Per-Arne on 24.02.2017.
//

#include "StateManager.h"
#include "../Game.h"


StateManager::StateManager(Game &game)
{
	walkingState = std::shared_ptr<BaseState>(new Walking(game));
	spawnState = std::shared_ptr<BaseState>(new Spawning());
	idleState = std::shared_ptr<BaseState>(new Idle());
	despawnedState = std::shared_ptr<BaseState>(new Despawned());
	harvestingState = std::shared_ptr<BaseState>(new Harvesting());
	buildingState = std::shared_ptr<BaseState>(new Building());
	combatState = std::shared_ptr<BaseState>(new Combat());
	deadState = std::shared_ptr<BaseState>(new Dead());

}


std::shared_ptr<BaseState> StateManager::getByID(int id){
    switch(id) {
        case Constants::State::Building:
            return buildingState;
        case Constants::State::Spawning:
            return spawnState;
        case Constants::State::Walking:
            return walkingState;
        case Constants::State::Idle:
            return idleState;
        case Constants::State::Despawned:
            return despawnedState;
        case Constants::State::Harvesting:
            return harvestingState;
        case Constants::State::Combat:
            return combatState;
        case Constants::State::Dead:
            return deadState;
        default:
            throw std::runtime_error( "Incorrect id of State. Expected a know state type!" );
    }
}
