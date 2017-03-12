//
// Created by Per-Arne on 24.02.2017.
//
#pragma once

#include "Unit.h"
#include "../Constants.h"
#include "../Config.h"
class Player;
class UnitManager {
public:

	static Unit constructUnit(Constants::Unit, Player *player);
	static Unit constructTownHall(Player *player);
	static Unit constructPeasant(Player *player);
	/*static void constructFarm(Unit &u);
	static void constructFootman(Unit &u);
	static void constructArcher(Unit &u);*/




};


