import math
import pygame
import numpy as np
import time

from Mechanics.Constants import Map as MapC
import itertools

class FakeTime:

    def __init__(self):
        self.multiplier = 1





class ArrayUtil:

    @staticmethod
    def neighbors(im, i, j, d=1):

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
    def neighbor_features_4(arr, i, j):
        possible_coords = [
            (i + 1, j),
            (i - 1, j),
            (i, j + 1),
            (i, j - 1)
        ]

        data = {
            "walkable": [],
            "harvestable": []
        }

        for possible_coord in possible_coords:
            tile_id = arr[possible_coord[0]][possible_coord[1]]
            tile = MapC.TILE_DATA[tile_id]
            if tile['ground']:
                data['walkable'].append(possible_coord)
            if tile['harvestable']:
                data['harvestable'].append(possible_coord)

        return data

    @staticmethod
    def walkable_cell_neighbors(arr, i, j, d):
        possible_coords = ArrayUtil.neighbors(arr, i, j, d)
        valid = []
        for possible_coord in possible_coords:
            tile_id = arr[possible_coord[0]][possible_coord[1]]
            tile = MapC.TILE_DATA[tile_id]
            if tile['ground']:
                valid.append(possible_coord)

        return valid



class SpriteUtil:
    def add_sprite(self, direction, x, y, width, height, tile_width=1, tile_height=1):
        w = tile_width * MapC.TILE_SIZE
        h = tile_height * MapC.TILE_SIZE

        sprite = SpriteUtil.get_sprite(self.spritesheet, x, y, width, height, w, h)
        self.sprite_map[direction].append(sprite)


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
