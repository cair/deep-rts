import itertools
from game.const import Unit, Map


def neighbors(i, j, d=1):
    xs = [x for x in range(i-d, i+d+1)]
    ys = [y for y in range(j-d, j+d+1)]
    neighbors = list(itertools.product(xs, ys))
    neighbors.remove((i, j))
    return neighbors


def get_area(x, y, width, height):
    map_tiles_x = [x for x in range(x, x + width)]
    map_tiles_y = [y for y in range(y, y + height)]
    all_tiles = list(itertools.product(map_tiles_x, map_tiles_y))
    return all_tiles


def adjacent_tiles(arr, i, j, d, type=Map.WALKABLE):
    possible_coord = neighbors(i, j, d)
    valid = []
    for possible_coord in possible_coord:
        tile_id = arr[possible_coord[0]][possible_coord[1]]
        tile = Map.TILE_DATA[tile_id]
        if tile['type'] == type:
            valid.append(possible_coord)

    return valid


def adjacent_walkable_tiles(unit, i, j, d, type=Map.WALKABLE):
    tiles = adjacent_tiles(unit.game.data['tile'], i, j, d,type)
    tiles = [x for x in tiles if is_walkable_tile(unit, *x)]
    return tiles


def is_walkable_tile(unit, x, y):
    tile_walkable = unit.game.data['tile'][x][y] == Map.GRASS
    unit_walkable = unit.game.data['unit'][x, y] == Unit.NONE

    return tile_walkable and unit_walkable
