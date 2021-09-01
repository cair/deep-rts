//
// Created by Per-Arne on 24.02.2017.
//
#pragma once
#include "Spawning.h"
#include "Walking.h"
#include "Idle.h"
#include "Despawned.h"
#include "Harvesting.h"
#include "Building.h"
#include "Combat.h"
#include "Dead.h"

class Game;
class StateManager {

public:
    explicit StateManager(Game &game);
	std::shared_ptr<BaseState> walkingState;
	std::shared_ptr<BaseState> spawnState;
	std::shared_ptr<BaseState> idleState;
	std::shared_ptr<BaseState> despawnedState;
	std::shared_ptr<BaseState> harvestingState;
	std::shared_ptr<BaseState> buildingState;
	std::shared_ptr<BaseState> combatState;
	std::shared_ptr<BaseState> deadState;

	std::shared_ptr<BaseState> getByID(int id);
};

