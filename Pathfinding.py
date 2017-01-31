from Mechanics.Constants import Map as MapC
from Mechanics.Map import Map
from Mechanics.Util import ArrayUtil
from PriorityQueue import PriorityQueue


def heuristic(a, b):

    (x1, y1) = a
    (x2, y2) = b
    return abs(x1 - x2) + abs(y1 - y2)


def crossover(current, start, goal):
    dx1 = current[0] - goal[0]
    dy1 = current[1] - goal[1]
    dx2 = start[0] - goal[0]
    dy2 = start[1] - goal[1]
    cross = abs(dx1*dy2 - dx2*dy1)
    return cross*0.001

def a_star_search(Map, graph, start, goal):

    frontier = PriorityQueue(None)
    frontier.put(start, 0)
    came_from = {}
    cost_so_far = {}
    came_from[start] = None
    cost_so_far[start] = 0

    while not frontier.empty():
        current = frontier.get()

        if current == goal:
            break

        neighbors = Map.AdjacentMap.adjacent_walkable(*current, 1)
        for next in neighbors:
            new_cost = cost_so_far[current]
            if next not in cost_so_far or new_cost < cost_so_far[next]:
                cost_so_far[next] = new_cost

                priority = new_cost + heuristic(goal, next) + crossover(next, start, goal)  # TODO is crossover required?
                frontier.put(next, priority)
                came_from[next] = current

    return list(reversed(determine_path(came_from, start, goal)))


def determine_path(came_from, start, goal):

    try:
        current = came_from[goal]
    except:
        return [] # No valid path

    path = [goal, current]
    while True:
        if came_from[current] is not None:
            current = came_from[current]

            path.append(current)
        else:
            end = path[::-1]
            return end

