#cython: language_level=3
import cv2
import numpy as np # TODO - cimport
cdef class Layer:

    cdef static;
    cdef data;
    cdef public gui;
    cdef public canvas;

    def __init__(self, gui, canvas, static=False, **kwargs):
        self.gui = gui
        self.static = static
        self.canvas = canvas

        # Build initial frame
        self.build()

    cpdef render(self):
        if self.static:
            return self.data
        self.build()
        return self.data

    cdef build(self):
        raise NotImplementedError("build is not implemented")

cdef class GroundLayer(Layer):
    cdef tile_width;
    cdef tile_height;

    def __init__(self, gui, canvas, **kwargs):
        self.tile_width = gui.map.tile_width
        self.tile_height = gui.map.tile_height

        super().__init__(gui, canvas)
        self.static = True  # TODO this should be TRUE

    cdef build(self):
        for tile in self.gui.tilemap.tiles:
            type = tile.get_type_id()
            tile_sprite = self.gui.sprite_manager.tiles[type]

            x_pos_start = tile.x * self.tile_width
            x_pos_end = x_pos_start + self.tile_width

            y_pos_start = tile.y * self.tile_height
            y_pos_end = y_pos_start + self.tile_height

            self.canvas[y_pos_start:y_pos_end, x_pos_start:x_pos_end] = tile_sprite

cdef class ResourceLayer(Layer):

    def __init__(self, gui, canvas, **kwargs):
        super().__init__(gui, canvas)

    cdef build(self):
        pass



cdef class UnitLayer(Layer):

    cdef map;
    cdef units;
    cdef sprite_manager;

    def __init__(self, gui, canvas, **kwargs):
        self.map = gui.map
        self.units = gui.game.units
        self.sprite_manager = gui.sprite_manager

        super().__init__(gui, canvas)

        self.static = False



    cdef build(self):

        for unit in self.units:

            # Unit is not located on map.
            if not unit.tile:
                continue

            x = unit.tile.x
            y = unit.tile.y
            width = unit.width
            height = unit.height
            health = unit.health
            health_max = unit.health_max
            health_p = health / health_max

            owner_id = unit.get_player().get_id()
            x_start, y_start = (x * self.map.tile_width, y * self.map.tile_height)
            x_end, y_end = (x_start + self.map.tile_width, y_start + self.map.tile_height)

            sprite = self.sprite_manager.sprites[unit.type_id].get(unit.direction, 0)

            self.canvas[y_start:y_end, x_start:x_end] = composite(sprite, self.canvas[y_start:y_end, x_start:x_end])


cdef public composite(fg, bg): # TODO - Move to util class and make it actually work.
    cdef a;
    cdef blended;

    a = (np.multiply(fg[:,:,3], 1.0 / 255))[:,:,np.newaxis]
    return cv2.convertScaleAbs(fg * a + bg * (1-a))

