import os

import numpy as np
import pygame


from Sprites import Sprites

dir_path = os.path.dirname(os.path.realpath(__file__))


class Tiles:

    def __init__(self):
        pass

    def set_tile(self, x, y, type):
        pass


class FogTile(pygame.sprite.DirtySprite):

    def __init__(self, group, x, y):
        pygame.sprite.DirtySprite.__init__(self, group)
        self.image = pygame.Surface([32, 32])
        self.image.fill(0)

        self.rect = self.image.get_rect()
        self.rect.x = x * 32
        self.rect.y = y * 32


class Fog:

    def __init__(self, width, height):
        self.group = pygame.sprite.LayeredDirty()
        self.hidden = list()
        self.width = width
        self.height = height

        for y in range(height):
            for x in range(width):
                FogTile(self.group, x, y)

    def draw(self, surface, game):

        if game.selected_player:
            for unit in game.units:

                if unit.tile and game.selected_player.get_id() == unit.get_player().get_id():

                    for h in range(-unit.sight, unit.height + unit.sight):
                        for w in range(-unit.sight, unit.width + unit.sight):
                            idx = max(0, (unit.tile.x + w)) + max(0, (unit.tile.y + h) * self.width)
                            sprite = self.group.get_sprite(idx)
                            sprite.visible = 0
                            sprite.dirty = 1
                            self.hidden.append(idx)

            return self.group.draw(surface)

    def reset(self):
        for idx in self.hidden:
            sprite = self.group.get_sprite(idx)
            sprite.visible = 1
            sprite.dirty = 1


class GUI:

    def __init__(self, game):
        self.game = game
        self.tilemap = self.game.tilemap
        self.map = self.game.map

        self.gui_tiles = Tiles()

        self.tiles = self.tilemap.tiles
        self.units = self.game.units
        self.fog = Fog(self.map.map_width, self.map.map_height)

        pygame.init()
        pygame.display.set_caption('DeepRTS v1.4.0')

        # Camera Variables
        self.camera_x = 0
        self.camera_y = 0

        # Window / Canvas Variables
        self.tilemap_size = (self.map.map_width, self.map.map_height)
        self.tile_width, self.tile_height = self.map.tile_width, self.map.tile_height
        self.tilemap_render_size = (self.map.map_width * self.map.tile_width, self.map.map_height * self.map.tile_height)
        self.window_size = self.tilemap_render_size  # (800, 800)
        self.display = pygame.display.set_mode(self.window_size)
        self.surface_map = pygame.Surface(self.tilemap_render_size)  # Tiles that may change during game

        # Load Resources
        self.unit_sprites, self.tile_sprites = Sprites(self).load()

        # Setup
        self.static_tiles = self.setup_static_map()
        self.dynamic_tiles = set(self.tiles) - set(self.static_tiles)

    def setup_static_map(self):
        static_tiles = []
        for tile in self.tiles:
            # Harvestable tiles can be transformed when depleted
            if tile.is_harvestable():
                continue

            tile_sprite = self.tile_sprites[tile.get_type_id()]
            self.surface_map.blit(tile_sprite, (tile.x * self.map.tile_width, tile.y * self.map.tile_height))
            static_tiles.append(tile)

        return static_tiles

    def render_tiles(self):

        for tile in self.tiles:
            x = tile.x
            y = tile.y
            type_id = tile.get_type_id()
            tile_sprite = self.tile_sprites[type_id]

            self.surface_map.blit(tile_sprite, (x * self.map.tile_width, y * self.map.tile_height))

    def render_units(self):

        for unit in self.game.units:
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
            x_pos, y_pos = (x * self.map.tile_width, y * self.map.tile_height)

            # Draw unit
            unit_sprite = self.unit_sprites[owner_id][unit.type_id][unit.direction][0]  # TODO Fix animation
            self.surface_map.blit(unit_sprite, (x_pos, y_pos, width * self.tile_width, height * self.tile_height))

            # Draw Healthbar
            pygame.draw.rect(self.surface_map, (0, 255, 0), (x_pos, y_pos - 6, (width * self.tile_width) * health_p, 5))
            if health_p != 1.0:
                pygame.draw.rect(
                    self.surface_map,
                    (255, 255, 255),
                    (x_pos + (width * self.tile_width),
                     y_pos - 6,
                     -(width * self.tile_width) * (1 - health_p),
                     5)
                )

    def render(self):
        self.fog.reset()

        self.render_tiles()
        self.render_units()

        if self.game.config.pomdp:
            self.fog.draw(self.surface_map, self.game)

        self.display.blit(self.surface_map, (0, 0))

    def view(self):
        if self.game.config.pomdp:
            pygame.display.update(self.fog.draw(self.surface_map, self.game))
        else:
            pygame.display.flip()

    def capture(self, save=False, filename="./capture.png"):
        if save:
            imsave(filename, np.array(pygame.surfarray.pixels3d(self.surface_map)))

        return np.array(pygame.surfarray.pixels3d(self.surface_map))

    def set_caption(self, param):
        pygame.display.set_caption(param)
