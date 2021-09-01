//
// Created by Per-Arne on 26.02.2017.
//

#ifndef WARC2SIM_PATHFINDER_H
#define WARC2SIM_PATHFINDER_H

#include <list>
#include <queue>
#include <unordered_map>
#include <vector>
#include <map>
#include <set>
class Tile;

class Pathfinder {

public:
	static bool aStar(std::vector<Tile *> &constructedPath, Tile *start, Tile *goal);

    static double heuristic(Tile *pTile, Tile *next);

    static double crossover(Tile *pTile, Tile *start, Tile *goal);

	static Tile * find_closest_walkable_tile(Tile * start, Tile * destination, int range);

    static Tile *find_first_walkable_tile(Tile *start);

    static Tile *find_first_harvestable_tile(Tile *pTile);


	static std::vector<Tile *> reconstruct_path(Tile *start, Tile *goal, std::unordered_map<Tile*, Tile*>& came_from);

    static Tile *find_first_attackable_tile(Tile *start);
};


#endif //WARC2SIM_PATHFINDER_H
