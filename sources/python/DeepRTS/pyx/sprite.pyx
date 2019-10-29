import os
import cv2
import numpy as np  # Import CIMPORT

cdef class Sprite:
    cdef gui;
    cdef sprite;
    cdef states;
    cdef iterator;
    cdef state;
    cdef directions;


    def __init__(self, gui, sprite, directions, **kwargs):
        self.gui = gui
        self.sprite = sprite
        self.directions = directions
        self.states = {
            direction: [] for direction in directions
        }
        # sprite.lower()
        spritesheet_path = os.path.join(os.getcwd(), "assets",  "textures", "peasant" + ".png")  # TODO cwd
        self.load(spritesheet_path)

    cdef load(self, filename):
        spritesheet = cv2.imread(filename, cv2.IMREAD_UNCHANGED)
        #spritesheet[:,:,3] = (np.multiply(spritesheet[:,:,3], 1.0 / 255))[:,:,np.newaxis]

        h, w, d = spritesheet.shape

        num_animations = int(w / self.gui.map.tile_width)

        for direction in self.directions:
            direction_id = int(direction)
            #print(direction_id, direction)
            y_start = direction_id * self.gui.map.tile_height
            y_end = y_start + self.gui.map.tile_height

            for x in range(0, num_animations):
                x_start = x * self.gui.map.tile_width
                x_end = x_start + self.gui.map.tile_width

                self.states[direction].append(
                    spritesheet[y_start:y_end, x_start:x_end]
                )
    cpdef public get(self, direction, iterator):  # TODO why not cdef?
        return self.states[direction][iterator]


    cdef set_state(self, state):
        self.iterator = 0
        self.state = state
