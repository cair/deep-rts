//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_CONSTANTS_H
#define WARC2SIM_CONSTANTS_H
#include <string>
#define DEBUG(x) do { std::cerr << x; } while (0)

namespace Constants{

	const int MAX_PLAYERS = 8;
	const int MAX_UNITS = 1000;


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

	const int action_min = 1;
	const int action_max = 16;

	const std::string actionNames[16] = {
		"PrevUnit",
		"NextUnit",


		"MoveLeft",
		"MoveRight",
		"MoveUp",
		"MoveDown",
		"MoveUpLeft",
		"MoveUpRight",
		"MoveDownLeft",
		"MoveDownRight",

		"Attack",
		"Harvest",

		"Build0",
		"Build1",
		"Build2",

		"NoAction",
	};

}






#endif //WARC2SIM_CONSTANTS_H
