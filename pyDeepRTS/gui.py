import os
import pygame
from pygame.locals import *
from pyDeepRTS.Sprites import Sprites


dir_path = os.path.dirname(os.path.realpath(__file__))


class GUI:

    def __init__(self, game):
        self.game = game
        self.map = self.game.map
        self.tiles = self.map.tiles
        self.units = self.game.units


        pygame.init()
        pygame.display.set_caption('DeepRTS v1.4.0')

        # Camera Variables
        self.camera_x = 0
        self.camera_y = 0

        # Window / Canvas Variables
        self.window_size = (800, 800)
        self.map_size = (self.map.map_width, self.map.map_height)
        self.map_render_size = (self.map.map_width * self.map.tile_width, self.map.map_height * self.map.tile_height)

        self.display = pygame.display.set_mode(self.window_size, pygame.DOUBLEBUF)
        self.surface_map = pygame.Surface(self.map_render_size, pygame.SWSURFACE)  # Tiles that may change during game


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



    def set_camera(self):
        try:
            player_unit = self.player.units[0]

            pix_x, pix_y = player_unit.x * Map.TILE_SIZE * -1, player_unit.y * Map.TILE_SIZE * -1

            self.camera_x = pix_x + (self.window_size[0] / 3)
            self.camera_y = pix_y + (self.window_size[1] / 4)

        except:
            pass

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
            # List of available sprites
            try:
                unit_sprite = self.unit_sprites[unit.type_id][unit.direction][0]  # TODO Fix animation
            except:
                print(unit.type_id, unit.direction)

            self.surface_map.blit(unit_sprite, (x * self.map.tile_width, y * self.map.tile_height))





        """
     


        # Render Sprite
        self.canvas.blit(sprite, pos)

        # Render healthbar
        health_percent = (unit.state.health / unit.health_max)
        pygame.draw.rect(self.canvas, (255, 0, 0),
                         (pos[0] - 10,
                          pos[1] - 5,
                          50,
                          5
                          ))
        pygame.draw.rect(self.canvas, (0, 255, 0),
                         (pos[0] - 10,
                          pos[1] - 5,
                          50 * health_percent,
                          5
                          ))

        # Render State List
        if Config.DEBUG:
            self.canvas.blit(
                Overlay.font.render('->'.join([x.id for x in unit.state.next_states]) + str(unit.spawned), 1, (255, 255, 0)),
                (unit.state.x * Map.TILE_SIZE, unit.state.y * Map.TILE_SIZE)
            )

        unit.animation_timer += dt
        if unit.animation_timer > unit.animation_interval and unit.state.id == ID_Walking:
            unit.animation_iterator += 1
            unit.animation_timer = 0
        """

    def render(self):

        self.render_tiles()
        self.render_units()

        self.display.blit(self.surface_map, (0, 0))


        pygame.display.flip()



