//
// Created by Per-Arne on 26.02.2017.
//

#include <list>
#include <queue>
#include <unordered_map>
#include <vector>
#include <map>
#include <set>
#include "Pathfinder.h"
#include "../environment/Tile.h"
#include "../util/PriorityQueue.hpp"
#include "../Constants.h"
#include "../environment/Tilemap.h"



bool Pathfinder::aStar(std::vector<Tile *> &constructedPath, Tile *start, Tile *goal)
{

    std::vector<Tile *> path;
    std::unordered_map<Tile*, Tile*> came_from;
    std::unordered_map<Tile*, double> cost_so_far;


    auto frontier = PriorityQueue<Tile*, double>();
    frontier.put(start, 0);


    came_from[start] = nullptr;
    cost_so_far[start] = 0;

    while (!frontier.empty()) {
        Tile *current = frontier.get();


		if (current == goal) {
			constructedPath = reconstruct_path(start, goal, came_from);
			return true;
		}


        std::vector<Tile*> neighbors = current->getTilemap().neighbors(*current, Constants::Pathfinding::Walkable);

        for (auto next : neighbors) {

            double new_cost = cost_so_far.at(current) + 1;
            if (!cost_so_far.count(next) || new_cost < cost_so_far.at(next)) {
                cost_so_far[next] = new_cost;
                double priority = new_cost + heuristic(goal, next) + crossover(next, start, goal);
                frontier.put(next, priority);
                came_from[next] = current;

            }
        }

    }

    return false;
}

double Pathfinder::heuristic(Tile *goal, Tile *next) {
    int dx = abs(next->x - goal->x);
    int dy = abs(next->y - goal->y);

    return (dx + dy) + (1 - 2 * 1) * std::min(dx, dy);


    return abs(goal->x - next->y) + abs(goal->y - next->y);
}

double Pathfinder::crossover(Tile *current, Tile *start, Tile *goal) {
    int dx1 = current->x - goal->x;
    int dy1 = current->y - goal->y;
    int dx2 = start->x - goal->x;
    int dy2 = start->y - goal->y;
    int cross = abs(dx1*dy2 - dx2*dy1);

    return cross*0.001;
}

Tile* Pathfinder::find_closest_walkable_tile(Tile *start, Tile *destination, int range) {

	std::set<Tile *> visited;
	std::queue<Tile *> queue;

	Tile *closest = nullptr;
	uint16_t tmpClosest = INT16_MAX;

	for (int dx = -1; dx <= range; dx++) {
		for (int dy = -1; dy <= range; dy++) {
			uint16_t x = destination->x + dx;
			uint16_t y = destination->y + dy;


			Tile &subject = destination->getTilemap().getTile(x, y);
			if (subject.getOccupant()) {
				continue;
			}

			uint16_t dist = start->distance(subject);
			if (dist < tmpClosest) {
				closest = &subject;
				tmpClosest = dist;
			}

		}
	}



	return closest;

}

Tile* Pathfinder::find_first_walkable_tile(Tile *start) {
    /**
     * This is a breadth-first search which looks for a walkable tile
     */

    std::set<Tile *> visited;
    std::queue<Tile *> queue;

    queue.push(start);

    while(queue.size() > 0) {

        Tile *current = queue.front();

        assert(current);
        queue.pop();

        if (current->isWalkable()) {

            return current;
        }


        const bool is_in = visited.find(current) != visited.end();
        if(!is_in) {
            visited.insert(current);
            std::vector<Tile*> neighbors = current->getTilemap().neighbors(*current, Constants::Pathfinding::All);
            for(auto &i : neighbors) {
                queue.push(i);
            }
        }
    }

    return nullptr;
    assert(false); // Should not be here. this means algorithm didnt find any closest tile and there should be one


}

Tile *Pathfinder::find_first_harvestable_tile(Tile *start) {
    /**
   * This is a breadth-first search which looks for a walkable tile
    */

    std::set<Tile *> visited;
    std::queue<Tile *> queue;

    queue.push(start);

    while(queue.size() > 0) {

        Tile *current = queue.front();

        queue.pop();

        if (current->isHarvestable()) {
            return current;
        }
        const bool is_in = visited.find(current) != visited.end();
        if(!is_in) {
            visited.insert(current);
            std::vector<Tile*> neighbors = current->getTilemap().neighbors(*current, Constants::Pathfinding::All);
            for(auto &i : neighbors) {
                queue.push(i);
            }
        }
    }

    return nullptr;
}

Tile *Pathfinder::find_first_attackable_tile(Tile *start) {
    /**
   * This is a breadth-first search which looks for a walkable tile
    */

    std::set<Tile *> visited;
    std::queue<Tile *> queue;

    queue.push(start);

    while(queue.size() > 0) {

        Tile *current = queue.front();

        queue.pop();

        if (current->isAttackable(*start->getOccupant())) {
            return current;
        }
        const bool is_in = visited.find(current) != visited.end();
        if(!is_in) {
            visited.insert(current);
            std::vector<Tile*> neighbors = current->getTilemap().neighbors(*current, Constants::Pathfinding::All);
            for(auto &i : neighbors) {
                queue.push(i);
            }
        }
    }

    return nullptr;
}


std::vector<Tile*> Pathfinder::reconstruct_path(Tile * start, Tile * goal, std::unordered_map<Tile*, Tile*>& came_from)
{
	std::vector<Tile *> path;
	Tile *current = goal;
	path.push_back(current);

	while (came_from.at(current) != start) {
		current = came_from.at(current);
		path.push_back(current);
	}
	//path.push_back(start); // optional
	//std::reverse(path.begin(), path.end());
	return path;
}
