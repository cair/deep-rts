import numpy as np
import cv2
import os
dir_path = os.path.dirname(os.path.realpath(__file__))

class OpenCVRenderer:

    def __init__(self, width, height, tile_width_x, tile_width_y):
        self.image = np.zeros((width * tile_width_x, height * tile_width_y, 3), np.uint8)

        self.S_GRASS = cv2.imread(os.path.join(dir_path, 'data', 'textures', 'grass.png'))
        self.S_GRASS = cv2.resize(self.S_GRASS, (tile_width_x, tile_width_y))

        self.S_TREE = cv2.imread(os.path.join(dir_path, 'data', 'textures', 'tree.png'))
        self.S_TREE = cv2.resize(self.S_GRASS, (tile_width_x, tile_width_y))

    def blit(self, x, y, type):
        pass

    def save(self, path):
        cv2.imwrite(path, self.image)



OpenCVRenderer(300, 300, 32, 32)