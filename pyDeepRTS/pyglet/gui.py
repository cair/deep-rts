import math
import pyglet
import numpy
from PIL import Image
from pyDeepRTS.util import get_sprite, image_at
from DeepRTS.Constants import Unit, Direction, Tile
import os
dir_path = os.path.dirname(os.path.realpath(__file__))


class Tiles:

    def __init__(self, game, gui):
        self.game = game
        self.gui = gui

        # Grass 4
        #

        self.tile_trans = {
            270: 4,
            142: 258,
            102: 285,
        }
        self.data = self.load_tiles()

        self.batch = pyglet.graphics.Batch()
        self.sprites = numpy.ndarray(shape=(game.get_width(), game.get_height()), dtype=numpy.object)
        self.s_types = numpy.zeros(shape=(game.get_width(), game.get_height()), dtype=numpy.uint8)

    def set_tile(self, x, y, type_id):
        tile = self.s_types[x, y]

        if tile == 0:
            # Insert new tile
            self.s_types[x, y] = type_id
            self.sprites[x, y] = pyglet.sprite.Sprite(
                self.data[self.tile_trans[type_id]],
                *self.gui.to_pygame(x * self.gui.tile_width, y * self.gui.tile_height),
                batch=self.batch
            )

        elif tile != type_id:
            self.s_types[x, y] = type_id
            self.sprites[x, y].delete()
            self.sprites[x, y] = pyglet.sprite.Sprite(
                self.data[self.tile_trans[type_id]],
                *self.gui.to_pygame(x * self.gui.tile_width, y * self.gui.tile_height),
                batch=self.batch
            )

    def load_tiles(self):
        tileset_path = os.path.join(dir_path, "..", "..", "data", "textures", "tiles.png")
        sheet = pyglet.image.load(tileset_path)

        cols = math.floor(sheet.width / self.gui.tile_width)
        rows = math.floor(sheet.height / self.gui.tile_height)

        img_grid = pyglet.image.ImageGrid(sheet, rows, cols, self.gui.tile_width, self.gui.tile_height, 1, 1)
        texture_grid = pyglet.image.TextureGrid(img_grid)
        return texture_grid

class Units:

    def __init__(self, game):
        self.game = game
        self.tile_width = self.game.map.tile_width
        self.tile_height = self.game.map.tile_height


        self.tile_trans = {
            270: 4,
            142: 258,
            102: 285,
        }

        self.data = self.load_tiles()

        self.batch = pyglet.graphics.Batch()
        self.sprites = numpy.ndarray(shape=(game.get_width(), game.get_height()), dtype=numpy.object)
        self.s_types = numpy.zeros(shape=(game.get_width(), game.get_height()), dtype=numpy.uint8)


class GUI:

    def to_pygame(self, x, y):
        """Convert an object's coords into pygame coordinates (lower-left of object => top left in pygame coords)."""
        return x, self.window.height - y - self.tile_height

    def __init__(self, game, capture_game_only=True, grayscale=False):

        self.game = game
        self.tile_width = self.game.map.tile_width
        self.tile_height = self.game.map.tile_height
        self.game_width = self.game.get_width() * self.game.map.tile_width
        self.game_height = self.game.get_height() * self.game.map.tile_height

        self.window = pyglet.window.Window(
            width=self.game_width,
            height=self.game_height,
            caption="DeepRTS v2.0",
            vsync=False,
        )
        self.gui_tiles = Tiles(game, self)

        self.capture_width =  self.game_width if capture_game_only else self.window.width
        self.capture_height =  self.game_height if capture_game_only else self.window.height
        self.buffer = (pyglet.gl.GLubyte * (3 * self.capture_width * self.capture_height))(0)

    def capture(self, save=False, filename="./pyglet.png"):

        """
        -(self.window.width - self.game_width),
            self.window.height - self.game_height,

        :param save:
        :param filename:
        :return:
        """
        pyglet.gl.glReadPixels(
            0, 0,
            self.capture_width,
            self.capture_height,
            pyglet.gl.GL_RGB,
            pyglet.gl.GL_UNSIGNED_BYTE,
            self.buffer
        )
        image = Image.frombuffer(
            "RGB",
            (self.capture_width, self.capture_height),
            self.buffer, "raw", "RGB", 0, 1)

        image = image.transpose(Image.ROTATE_90)

        if save:
            image.save(filename)

        return image

    def view(self):
        self.window.flip()
        self.window.dispatch_events()

    def render(self):
        self.window.clear()
        self.gui_tiles.batch.draw()

    def set_caption(self, text):
        self.window.set_caption(text)


