import pygame


class RectangleManager:

    def __init__(self, map_height, map_width, tile_size):

        self.rectangles = [
            pygame.Rect(
                (x * tile_size, y * tile_size),
                (tile_size, tile_size)
            ) for y in range(map_height) for x in range(map_width)
        ]

        self.changed_tiles = []
        self.changed_rects = []

    def add_changed_tile(self, tile):
        self.changed_tiles.append(tile)

    def clear_changed_tile(self):
        self.changed_tiles = []

    def add_changed_rect(self, rect):
        self.changed_rects.append(rect)

    def clear_changed_rect(self):
        self.changed_rects = []

    def full_refresh(self):
        self.changed_rects.extend(self.rectangles)