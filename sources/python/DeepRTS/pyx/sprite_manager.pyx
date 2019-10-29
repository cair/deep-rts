#cython: language_level=3
import cv2
import math
import os

from pyDeepRTS.Constants import Unit, Direction, Tile
from sprite import Sprite

cdef class SpriteManager:
    cdef gui;
    cdef color_mask;  # Color mask
    cdef public sprites;
    cdef public tiles;

    def __init__(self, gui, **kwargs):
        self.gui = gui


        self.color_mask = {
            0: (255, 0, 0),  # Player 1
            1: (0, 0, 255),
            2: (0, 255, 0),
            3: (255, 255, 0),
            4: (0, 255, 255),
            5: (255, 0, 255)  # Player 6
        }

        tile_types = [int(getattr(Tile, x)) for x in Tile.__dict__ if x[0].isupper()]
        unit_classes = [getattr(Unit, x) for x in Unit.__dict__ if x[0].isupper()]
        directions = [getattr(Direction, x) for x in Direction.__dict__ if x[0].isupper()]

        tileset_path = os.path.join(os.getcwd(), "assets", "textures", "tiles.png")  # TODO cwd
        tileset = cv2.imread(tileset_path)
        tileset = cv2.cvtColor(tileset, cv2.COLOR_RGB2RGBA)
        tileset[:, :, 3] = 255

        self.tiles = {
            tile_type: self.sub_rect(tileset, tile_type , self.gui.map.tile_width)
            for tile_type in tile_types
        }

        self.sprites = {
            sprite: Sprite(gui=gui, sprite=sprite, directions=directions) for sprite in unit_classes
        }


    cpdef sub_rect(self, tileset, tile_n, tile_size):
        size = tileset.shape
        n_cols = math.floor(size[0] / tile_size)
        n_rows = math.floor(tile_n / n_cols)
        mod_id = tile_n % n_cols

        x_start = (mod_id * tile_size) + mod_id
        y_start = (n_rows * tile_size) + n_rows

        return tileset[y_start:y_start+tile_size, x_start:x_start+tile_size]
