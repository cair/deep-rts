//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_CONSTANTS_H
#define WARC2SIM_CONSTANTS_H

namespace Constants{
	enum Unit {
		Peasant = 1,
		Peon = 2,
		TownHall = 3
	};


	enum Race {
		Human = 1,
		Orc = 2
	};


	enum Tile {
		Spawn = 16,
		Grass = 271
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
		None = -1
	};

	enum Pathfinding {
		Walkable = 1,
		All = 2
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
		RightLeft = 2,
		RightRight = 3,
		RightUp = 4,
		RightDown = 5,
		RightUpLeft = 6,
		RightUpRight = 7,
		RightDownLeft = 8,
		RightDownRight = 9,
		NextUnit = 10,
		Build0 = 11,
		Build1 = 12,
		Build2 = 13,
		NoAction = 14
	};

}






#endif //WARC2SIM_CONSTANTS_H
