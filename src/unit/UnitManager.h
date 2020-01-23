//
// Created by Per-Arne on 24.02.2017.
//
#pragma once

#include "Unit.h"
#include "../Constants.h"
class Player;
class UnitManager {
public:

	static Unit constructUnit(Constants::Unit unitType, Player &player);
	static void updateUnitCount(Player &p, Constants::Unit unitType, int n);
	static Unit constructTownHall(Player &player);
	static Unit constructPeasant(Player &player);
	static Unit constructFarm(Player &player);
	static Unit constructBarracks(Player &player);
    static Unit constructFootman(Player &player);
    static Unit constructArcher(Player &player);
};


