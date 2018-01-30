import os
import random
import sys
import pygame
from pygame.locals import *

from pyDeepRTS.Sprites import Sprites
from .util import get_sprite, image_at



dir_path = os.path.dirname(os.path.realpath(__file__))





class GUIState:

    def __init__(self):
        self.SCROLL_DOWN = False
        self.SCROLL_LEFT = False
        self.SCROLL_RIGHT = False
        self.SCROLL_UP = False
        self.selected_unit = None


class Button:

    def __init__(self, gui, unit, x, y, width=180, height=15):
        self.width = width
        self.height = height
        self.x = x
        self.y = y
        font = pygame.font.SysFont("monospace", 15)
        lbl_text = font.render("%s [%s|%s]" % (unit.name, unit.cost_lumber, unit.cost_gold), 1, (0, 0, 0))

        pygame.draw.rect(gui.display, (255, 255, 255), (x, y, width, height))
        gui.display.blit(lbl_text, (x, y))


class Overlay:
    font = None
    font_h1 = None
    small = None

    overlay = None

    @staticmethod
    def init(gui):
        Overlay.font = pygame.font.SysFont("monospace", 15)
        Overlay.small = pygame.font.SysFont("arial", 10)
        Overlay.font_h1 = pygame.font.SysFont("monospace", 24)

        # Overlay surface
        overlay = pygame.Surface(gui.window_size)

        # Panel background
        pygame.draw.rect(
            overlay,
            pygame.locals.Color('black'),
            (0, 0, gui.window_size[0], 25)
        )

        # Border
        pygame.draw.rect(
            overlay,
            pygame.locals.Color('green'),
            (0, gui.display.get_height() - 102, gui.window_size[0], 10)
        )
        # Panel background
        pygame.draw.rect(
            overlay,
            pygame.locals.Color('black'),
            (0, gui.display.get_height() - 100, gui.window_size[0], 100)
        )

        overlay = overlay.convert_alpha()
        Overlay.overlay = overlay


    @staticmethod
    def render_top_panel(gui):
        """
        TOP PANEL
        """
        lbl = Overlay.font.render(
            "Lumber %s "
            "Gold: %s "
            "Food %s/%s "
            "Units: %s "
            "Seconds: %s  "
            "Frames: %s "
            "Score: %s "
            % (
                gui.player.lumber,
                gui.player.gold,
                gui.player.consumed_food, gui.player.food,
                len(gui.player.units),
                int(gui.game.clock.elapsed_update / Config.FRAME_MULTIPLIER),
                int(gui.game.clock.elapsed_update),
                int(gui.player.score_total())
            ), 1, (255, 255, 0))
        gui.display.blit(lbl, (10, 5))

    @staticmethod
    def render_bottom_panel(gui):
        """
        BOTTOM PANEL
        """
        if not gui.gstate.selected_unit:
            return

        lbl_unit_name = Overlay.font_h1.render("%s (%s) - %s (%s,%s)" %
                                               (
                                                   gui.gstate.selected_unit.name,
                                                   gui.gstate.selected_unit.unit_id,
                                                   gui.gstate.selected_unit.state.id,
                                                   gui.gstate.selected_unit.state.x,
                                                   gui.gstate.selected_unit.state.y
                                               ), 1, (255, 255, 0))

        line_1 = "Lumber: %s Health: %s/%s   Ground: %s" % (
            gui.gstate.selected_unit.state.inventory_lumber,
            gui.gstate.selected_unit.state.health, gui.gstate.selected_unit.health_max,
            gui.gstate.selected_unit.ground_unit
        )

        line_2 = "Gold: %s   Damage: %s - %s   Water: %s" % (
            gui.gstate.selected_unit.state.inventory_gold,
            gui.gstate.selected_unit.damage_min, gui.gstate.selected_unit.damage_max,
            gui.gstate.selected_unit.water_unit,
        )

        line_3 = "Oil: %s    Armor: %s        Speed: %s" % (
            gui.gstate.selected_unit.state.inventory_oil,
            gui.gstate.selected_unit.armor,
            gui.gstate.selected_unit.speed
        )

        buttons = []
        thestr = ""
        for building in gui.gstate.selected_unit.buildable:
            buttons.append(building)
            thestr += "%s [%s|%s]" % (building.name, building.cost_lumber, building.cost_gold)

        gui.display.blit(lbl_unit_name, (200, 500))
        gui.display.blit(Overlay.font.render(line_1, 1, (255, 255, 0)), (150, 525))
        gui.display.blit(Overlay.font.render(line_2, 1, (255, 255, 0)), (150, 545))
        gui.display.blit(Overlay.font.render(line_3, 1, (255, 255, 0)), (150, 565))
        gui.display.blit(Overlay.font.render(thestr, 1, (255, 255, 0)), (150, 585))


class GUI:

    def __init__(self, game):
        self.game = game
        self.map = self.game.get_tilemap()
        self.tiles = self.map.tiles

        print(dir(self.game))
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

        self.display = pygame.display.set_mode(self.window_size)
        self.surface_map = pygame.Surface(self.map_render_size)  # Tiles that may change during game


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


            print(tile.get_name(), tile.is_depleted(), tile.get_type_id(), tile.get_resources())

            self.surface_map.blit(tile_sprite, (x * self.map.tile_width, y * self.map.tile_height))

    def render_units(self):

        for unit in self.game.units:
            if not unit.tile:
                continue

            x = unit.tile.x
            y = unit.tile.y
            # List of available sprites
            unit_sprite = self.unit_sprites[unit.type_id][unit.direction][0]  # TODO Fix animation

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
        pygame.display.update()
        """
        self.process()

        # Draw Tile map
        self.render_tiles()
        self.render_units(dt)

        self.display.blit(self.canvas, (self.camera_x, self.camera_y))


        Overlay.render_top_panel(self)
        Overlay.render_bottom_panel(self)

        #
        """

    def caption(self, dt):
        pygame.display.set_caption(
            ' '.join(('WarC2Sim', '[FPS=%d UPS=%d MUL=x%d]')) % (
                self.game.clock.fps,
                self.game.clock.ups,
                (self.game.clock.ups / Config.FRAME_MULTIPLIER)))


    def reset(self):
        self.gstate.selected_unit = None