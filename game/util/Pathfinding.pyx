from game.util.PriorityQueue import PriorityQueue



cdef double heuristic(int a_x, int a_y, int b_x, int b_y):
    return abs(a_x - b_x) + abs(a_y - b_y)


cdef double crossover(int curr_x, int curr_y, int start_x, int start_y, int goal_x, int goal_y):

    cdef int dx1 = curr_x - goal_x
    cdef int dy1 = curr_y - goal_y
    cdef int dx2 = start_x - goal_x
    cdef int dy2 = start_y - goal_y
    cdef int cross = abs(dx1*dy2 - dx2*dy1)

    return cross*0.001

def a_star_search(Map, graph, start, goal):

    frontier = PriorityQueue([])
    frontier.put(start, 0)
    came_from = {}
    cost_so_far = {}
    came_from[start] = None
    cost_so_far[start] = 0

    goal_x = goal[0]
    goal_y = goal[1]

    start_x = start[0]
    start_y = start[1]

    while not frontier.empty():
        current = frontier.get()

        if current == goal:
            break

        neighbors = Map.AdjacentMap.adjacent_walkable(*current, 1)
        for next in neighbors:
            new_cost = cost_so_far[current]
            if next not in cost_so_far or new_cost < cost_so_far[next]:
                cost_so_far[next] = new_cost
                next_x = next[0]
                next_y = next[1]

                priority = new_cost + \
                           heuristic(goal_x, goal_y, next_x, next_y) + \
                           crossover(next_x, next_y, start_x, start_y, goal_x, goal_y)  # TODO is crossover required?
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

