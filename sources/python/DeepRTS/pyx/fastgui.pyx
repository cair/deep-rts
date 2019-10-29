#cython: language_level=3
import cv2
import numpy as np  # CIMPORT  TODO

from sprite_manager import SpriteManager
from layer import GroundLayer, ResourceLayer, UnitLayer

cdef class GUI:

    cdef public game;
    cdef public tilemap;
    cdef public map;
    cdef public tiles;
    cdef public units;
    cdef public layers;

    cdef public tilemap_size;
    cdef public tile_width, tile_height;
    cdef public tilemap_render_size;
    cdef public window_size;
    cdef public sprite_manager;

    cdef layer_ground, layer_unit, layer_resource;

    cdef canvas;
    cdef canvas_frames;

    def __init__(self, game, **kwargs):
        self.game = game
        self.tilemap = self.game.tilemap
        self.map = self.game.map

        self.tiles = self.tilemap.tiles
        self.units = self.game.units

        # SpriteManager
        self.sprite_manager = SpriteManager(gui=self)

        # Size of the map. typically 10x10, 15x15 etc
        self.tilemap_size = (self.map.map_width, self.map.map_height)

        # Size of a tile, typically 32x32
        self.tile_width, self.tile_height = self.map.tile_width, self.map.tile_height

        # Net size of the canvas (map * tile size)
        canvas_size = (self.map.map_width * self.map.tile_width, self.map.map_height * self.map.tile_height)

        # Create canvas
        canvas = np.zeros(shape=canvas_size + (4, ))
        self.canvas = canvas

        self.layer_ground = GroundLayer(gui=self, canvas=canvas, object=False)
        self.layer_resource= ResourceLayer(gui=self, canvas=canvas, object=True)
        self.layer_unit = UnitLayer(gui=self, canvas=canvas, object=True)


        self.layers = [self.layer_ground, self.layer_unit]
        #self.canvas_frames = list(reversed([canvas[c] for c in range(canvas.shape[0])]))

    cpdef render(self):
        for layer in self.layers:
            layer.render()

        cv2.imwrite("image.png", self.canvas)

    cpdef view(self):
        pass

    cpdef set_caption(self, caption):
        pass

cdef public composite(fg, bg): # TODO - Move to util class and make it actually work.
    cdef a;
    cdef blended;

    a = (np.multiply(fg[:,:,3], 1.0 / 255))[:,:,np.newaxis]
    return cv2.convertScaleAbs(fg * a + bg * (1-a))
