#cython: language_level=3
import pygame
import math
import numpy as np
import cv2

cdef class Util:

    @staticmethod
    cdef public composite(fg, bg):
        a = fg[:, :, 3, np.newaxis]
        #a = (np.multiply(fg[:,:,3], 1.0 / 255))[:,:,np.newaxis]
        blended = cv2.convertScaleAbs(fg * a + bg * (1-a))
        return blended


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

import os
import shutil
import stat
def copytree(src, dst, symlinks = False, ignore = None):

    if not os.path.exists(dst):
        os.makedirs(dst)
        shutil.copystat(src, dst)
        ignore = None
    lst = os.listdir(src)
    if ignore:
        excl = ignore(src, lst)
        lst = [x for x in lst if x not in excl]
    for item in lst:
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if symlinks and os.path.islink(s):
            if os.path.lexists(d):
                os.remove(d)
            os.symlink(os.readlink(s), d)
            try:
                st = os.lstat(s)
                mode = stat.S_IMODE(st.st_mode)
                os.lchmod(d, mode)
            except:
                pass  # lchmod not available
        elif os.path.isdir(s):
            copytree(s, d, symlinks, ignore)
        else:
            try:
                shutil.copy2(s, d)
            except:
                pass

def get_sprite(
        sheet,
        x,
        y,
        width,
        height,
        w_to,
        h_to,
        flip=False
):
    w_to = w_to * 32 # TODO
    h_to = h_to * 32 # TODO

    rect = pygame.Rect((x, y, width, height))
    image = pygame.Surface(rect.size, pygame.SRCALPHA, 32).convert_alpha()

    image.blit(sheet, (0, 0), rect)
    image = pygame.transform.flip(image, flip, False)
    image = pygame.transform.scale(image, (int(w_to), int(h_to)))

    return image
