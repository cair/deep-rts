//
// Created by Per-Arne on 26.02.2017.
//

#ifndef WARC2SIM_PATHFINDER_H
#define WARC2SIM_PATHFINDER_H


#include <vector>
class Tile;

class Pathfinder {

public:
    static std::vector<Tile *> aStar(Tile *start, Tile *end);

    static double heuristic(Tile *pTile, Tile *next);

    static double crossover(Tile *pTile, Tile *start, Tile *goal);

    static Tile *find_first_walkable_tile(Tile *start);

    static Tile *find_first_harvestable_tile(Tile *pTile);

};


#endif //WARC2SIM_PATHFINDER_H
