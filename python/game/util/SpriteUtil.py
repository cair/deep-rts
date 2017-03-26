import pygame
import math
from game.const import Map


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


def get_sprite(sheet, x, y, width, height, w_to, h_to, flip=False):
    w_to = w_to * Map.TILE_SIZE
    h_to = h_to * Map.TILE_SIZE

    rect = pygame.Rect((x, y, width, height))
    image = pygame.Surface(rect.size, pygame.SRCALPHA, 32).convert_alpha()

    image.blit(sheet, (0, 0), rect)
    image = pygame.transform.flip(image, flip, False)
    image = pygame.transform.scale(image, (int(w_to), int(h_to)))

    return image
