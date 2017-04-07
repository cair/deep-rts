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
	static Unit constructFarm(Player *player);
	static Unit constructBarracks(Player *player);
    static Unit constructFootman(Player *player);
    static Unit constructArcher(Player *player);

    /*static void constructFootman(Unit &u);
    static void constructArcher(Unit &u);*/





};


