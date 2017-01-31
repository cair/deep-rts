import math
import pygame

from Mechanics.Constants import Map as MapC
from Mechanics.Constants import Unit as UnitC
import itertools


class ArrayUtil:

    @staticmethod
    def neighbors(i, j, d=1):
        xs = [x for x in range(i-d, i+d+1)]
        ys = [y for y in range(j-d, j+d+1)]
        neighbors = list(itertools.product(xs, ys))
        neighbors.remove((i, j))
        return neighbors


    @staticmethod
    def get_area(x, y, width, height):
        map_tiles_x = [x for x in range(x, x + width)]
        map_tiles_y = [y for y in range(y, y + height)]
        all_tiles = list(itertools.product(map_tiles_x, map_tiles_y))
        return all_tiles

    @staticmethod
    def adjacent_tiles(arr, i, j, d, type=MapC.WALKABLE):
        possible_coord = ArrayUtil.neighbors(i, j, d)
        valid = []
        for possible_coord in possible_coord:
            tile_id = arr[possible_coord[0]][possible_coord[1]]
            tile = MapC.TILE_DATA[tile_id]
            if tile['type'] == type:
                valid.append(possible_coord)

        return valid

    @staticmethod
    def adjacent_walkable_tiles(unit, i, j, d, type=MapC.WALKABLE):
        tiles = ArrayUtil.adjacent_tiles(unit.game.data['tile'], i, j, d,type)
        tiles = [x for x in tiles if ArrayUtil.is_walkable_tile(unit, *x)]
        return tiles

    @staticmethod
    def is_walkable_tile(unit, x, y):
        tile_walkable = unit.game.data['tile_collision'][x][y] == MapC.WALKABLE
        unit_walkable = unit.game.data['unit'][x][y] == UnitC.NONE

        return tile_walkable and unit_walkable


class SpriteUtil:

    @staticmethod
    def image_at(sheet, tile_n, tile_size=32):
        size = sheet.get_size()
        n_cols = math.floor(size[0] / tile_size)
        n_rows = math.floor(tile_n / n_cols)
        mod_id = tile_n % n_cols
    
        x_start = (mod_id * tile_size) + mod_id
        y_start = (n_rows * tile_size) + n_rows
    
        rect = pygame.Rect((x_start, y_start, tile_size, tile_size))
        image = pygame.Surface(rect.size).convert()
        image.blit(sheet, (0, 0), rect)

        return image

    @staticmethod
    def get_sprite(sheet, x, y, width, height, w_to, h_to, flip=False):
        w_to = w_to * MapC.TILE_SIZE
        h_to = h_to * MapC.TILE_SIZE

        rect = pygame.Rect((x, y, width, height))
        image = pygame.Surface(rect.size, pygame.SRCALPHA, 32).convert_alpha()

        image.blit(sheet, (0, 0), rect)
        image = pygame.transform.flip(image, flip, False)
        image = pygame.transform.scale(image, (int(w_to), int(h_to)))

        return image
