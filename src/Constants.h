//
// Created by Per-Arne on 24.02.2017.
//
#pragma once

#include <string>
#include <map>

#define DEBUG(x) do { std::cerr << x; } while (0)

namespace Constants{

	const int MAX_PLAYERS = 8;
	const int MAX_UNITS = 1000;
    const int ACTION_MIN = 1;
    const int ACTION_MAX = 16; // Set based on last action in enum


	enum Unit {
		Peasant = 1,
		Peon = 2,
		TownHall = 3,

		Barracks = 4,
		Footman = 5,
		Farm = 6,
		Archer = 7,

		None = -1
	};


	enum Race {
		Human = 1,
		Orc = 2
	};


	enum Tile {
		Spawn = 17,
		Grass = 270,
		Wall = 52,
		Lumber = 142,
		Water = 334,
		Gold = 102,

	};


    const std::map<int, int> TypeToID = {
            {Spawn, 1},
            {Grass, 2},
            {Wall, 3},
            {Lumber, 4},
            {Water, 5},
            {Gold, 6},
    };


	enum State {
		Idle = 8,
		Spawning = 1,
		Walking = 2,
		Despawned = 3,
		Harvesting = 4,
		Building = 5,
		Combat = 6,
		Dead = 7,
		Base = -1,
	};

	enum Pathfinding {
		Walkable = 1,
		All = 2,
		Attackable = 3,
		Harvestable = 4
	};

	enum Direction {
		Down = 1,
		Up = 2,
		Left = 3,
		Right = 4,
		DownLeft = 5,
		DownRight = 6,
		UpLeft = 7,
		UpRight = 8
	};

	enum Action {
		PreviousUnit = 1,
		NextUnit = 2,

		MoveLeft = 3,
		MoveRight = 4,
		MoveUp = 5,
		MoveDown = 6,
		MoveUpLeft = 7,
		MoveUpRight = 8,
		MoveDownLeft = 9,
		MoveDownRight = 10,
		
		Attack = 11,
		Harvest = 12,

		Build0 = 13,
		Build1 = 14,
		Build2 = 15,
		NoAction = 16,
	};

    const std::map<int, std::string> ActionToName = {
            {PreviousUnit, "Prev Unit"},
            {NextUnit, "Next Unit"},
            {MoveLeft, "Move Left"},
            {MoveRight, "Move Right"},
            {MoveUp, "Move Up"},
            {MoveDown, "Move Down"},
            {MoveUpLeft, "Move Up Left"},
            {MoveUpRight, "Move Up Right"},
            {MoveDownLeft, "Move Down Left"},
            {MoveDownRight, "Move Down Right"},
            {Attack, "Attack"},
            {Harvest, "Harvest"},
            {Build0, "Build 0"},
            {Build1, "Build 1"},
            {Build2, "Build 2"},
            {NoAction, "No Action"}
    };


}