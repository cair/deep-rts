import os
import random
import sys

import pygame
from pygame.locals import *

from game import Config
from game.const.State import ID_Walking
from game.logic.Unit.Barracks import Barracks

from game.const import State
from game.const import Map
from game.const import Race
from game.const import Unit
from game.const import Input
from game.logic.Unit.Farm import Farm
from game.logic.Unit.Footman import Footman
from game.logic.Unit.Peasant import Peasant
from game.logic.Unit.TownHall import TownHall
from game.util import SpriteUtil

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

    def __init__(self, game, player):
        self.camera_x = 0
        self.camera_y = 0
        self.game = game
        self.player = player
        self.Map = self.game.Map
        self.gstate = GUIState()

        # Calculate window size
        self.canvas_size = (self.Map.width * Map.TILE_SIZE, self.Map.height * Map.TILE_SIZE)
        self.window_size = (800, 600)

        pygame.init()
        pygame.display.set_caption('WarC2Sim')


        # Canvas's
        self.display = pygame.display.set_mode(self.window_size) # Window
        self.canvas = pygame.Surface(self.canvas_size)
        Overlay.init(self)


        self.tiles_sprite = self.tile_sprites()  # tile sprites (images)
        self.unit_sprite = self.unit_sprites()

        self.tiles_drawn = False

        self.set_camera()

    def set_camera(self):
        try:
            player_unit = self.player.units[0]

            pix_x, pix_y = player_unit.x * Map.TILE_SIZE * -1, player_unit.y * Map.TILE_SIZE * -1

            self.camera_x = pix_x + (self.window_size[0] / 3)
            self.camera_y = pix_y + (self.window_size[1] / 4)

        except:
            pass

    def tile_sprites(self):
        tileset_path = os.path.abspath(os.path.join(dir_path, '../data/textures', self.Map.TILES_THEME, "tiles.png"))
        sheet = pygame.image.load(tileset_path).convert()

        result = [[None for x in range(self.Map.width)] for y in range(self.Map.height)]

        # Predefine sprites for each tile
        for x in range(self.Map.height):
            for y in range(self.Map.width):
                tile_type = self.game.data['tile'][x][y]
                tile_info = Map.TILE_DATA[tile_type]
                tile_choice = random.choice(tile_info['id'])
                sprite = SpriteUtil.image_at(sheet, tile_choice, Map.TILE_SIZE).convert()
                result[x][y] = sprite

        return result

    def unit_sprites(self):
        """
        {
            Unit.LEFT: [],
            Unit.UP: [],
            Unit.RIGHT: [],
            Unit.DOWN: [],
            Unit.UP_LEFT: [],
            Unit.UP_RIGHT: [],
            Unit.DOWN_LEFT: [],
            Unit.DOWN_RIGHT: []
        }
        :return:
        """

        Unit.PEASANT_SPRITE = Unit.PEASANT_SPRITE.convert_alpha()

        sprites = {
            Unit.PEASANT:  {
                Unit.LEFT: [
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 85, 4, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 85, 42, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 85, 82, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 85, 118, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 85, 155, 32, 32, Peasant.width, Peasant.height, True)
                ],
                Unit.UP: [
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 15, 4, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 15, 42, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 15, 82, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 15, 118, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 15, 155, 32, 32, Peasant.width, Peasant.height)
                ],
                Unit.RIGHT: [
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 85, 4, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 85, 42, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 85, 82, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 85, 118, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 85, 155, 32, 32, Peasant.width, Peasant.height)
                ],
                Unit.DOWN: [
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 164, 4, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 164, 42, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 164, 82, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 164, 118, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 164, 155, 32, 32, Peasant.width, Peasant.height)
                ],
                Unit.UP_LEFT: [
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 50, 4, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 50, 42, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 50, 82, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 50, 118, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 50, 155, 32, 32, Peasant.width, Peasant.height, True)
                ],
                Unit.UP_RIGHT: [
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 50, 4, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 50, 42, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 50, 82, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 50, 118, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 50, 155, 32, 32, Peasant.width, Peasant.height)
                ],
                Unit.DOWN_LEFT: [
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 120, 4, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 120, 42, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 120, 82, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 120, 118, 32, 32, Peasant.width, Peasant.height, True),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 120, 155, 32, 32, Peasant.width, Peasant.height, True)
                ],
                Unit.DOWN_RIGHT: [
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 120, 4, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 120, 42, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 120, 82, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 120, 118, 32, 32, Peasant.width, Peasant.height),
                    SpriteUtil.get_sprite(Unit.PEASANT_SPRITE, 120, 155, 32, 32, Peasant.width, Peasant.height)
                ]
            },
            Unit.FOOTMAN:  {
                Unit.LEFT: [
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 170, 7, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 170, 56, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 170, 99, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 170, 138, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 170, 176, 32, 32, Footman.width, Footman.height, True)
                ],
                Unit.UP: [
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 21, 7, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 21, 56, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 21, 99, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 21, 138, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 21, 176, 32, 32, Footman.width, Footman.height)
                ],
                Unit.RIGHT: [
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 170, 7, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 170, 56, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 170, 99, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 170, 138, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 170, 176, 32, 32, Footman.width, Footman.height)
                ],
                Unit.DOWN: [
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 315, 7, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 315, 56, 32, 36, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 315, 99, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 315, 138, 32, 36, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 315, 176, 32, 32, Footman.width, Footman.height)
                ],
                Unit.UP_LEFT: [
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 96, 7, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 96, 56, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 96, 99, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 96, 138, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 96, 176, 32, 32, Footman.width, Footman.height, True)
                ],
                Unit.UP_RIGHT: [
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 96, 7, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 96, 56, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 96, 99, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 96, 138, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 96, 176, 32, 32, Footman.width, Footman.height)
                ],
                Unit.DOWN_LEFT: [
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 241, 7, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 241, 56, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 241, 99, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 241, 138, 32, 32, Footman.width, Footman.height, True),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 241, 176, 32, 32, Footman.width, Footman.height, True)
                ],
                Unit.DOWN_RIGHT: [
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 241, 7, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 241, 56, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 241, 99, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 241, 138, 32, 32, Footman.width, Footman.height),
                    SpriteUtil.get_sprite(Unit.FOOTMAN_SPRITE, 241, 176, 32, 32, Footman.width, Footman.height)
                ],

            },
            Unit.TOWN_HALL:  {
                Unit.DOWN: [
                    SpriteUtil.get_sprite(Unit.BUILDING_SPRITESHEET, 270, 156, 111, 93, TownHall.width, TownHall.height),
                ],
                Unit.UP: [
                    SpriteUtil.get_sprite(Unit.BUILDING_SPRITESHEET, 270, 20, 111, 93, TownHall.width, TownHall.height)
                ]
            },
            Unit.FARM:  {
                Unit.DOWN: [
                    SpriteUtil.get_sprite(Unit.BUILDING_SPRITESHEET, 398, 70, 66, 66, Farm.width, Farm.height)
                ],
                Unit.UP: [
                    SpriteUtil.get_sprite(Unit.BUILDING_SPRITESHEET, 398, 1, 66, 66, Farm.width, Farm.height),
                ]
            },
            Unit.BARRACKS:  {
                Unit.DOWN: [
                    SpriteUtil.get_sprite(Unit.BUILDING_SPRITESHEET, 304, 560, 102, 102, Barracks.width, Barracks.height)
                ],
                Unit.UP: [
                    SpriteUtil.get_sprite(Unit.BUILDING_SPRITESHEET, 304, 457, 102, 102, Barracks.width, Barracks.height),
                ]
            }
        }

        return sprites

    def render_tiles(self):

        pygame.draw.rect(self.canvas, (0, 0, 0), (0, 0, self.Map.height * Map.TILE_SIZE, self.Map.width * Map.TILE_SIZE))

        for x, y in self.player.vision:
            self.canvas.blit(self.tiles_sprite[x][y], (x * Map.TILE_SIZE, y * Map.TILE_SIZE))

            # Render State List
            if Config.DEBUG:
                self.canvas.blit(
                    Overlay.small.render("(%s,%s)" % (x,y), 1, (255, 255, 0)),
                    (x * Map.TILE_SIZE, y * Map.TILE_SIZE)
                )

    def render_units(self, dt):

        for unit in self.game.units.values():

            coordinates = (unit.state.x, unit.state.y)

            if not unit.state.x or coordinates not in self.player.vision:
                continue

            sprite_arr = self.unit_sprite[unit.id][unit.state.direction] # Selection
            sprite = sprite_arr[unit.animation_iterator % len(sprite_arr)] # Selection based on frame
            pos = (unit.state.x * Map.TILE_SIZE, unit.state.y * Map.TILE_SIZE)

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

    def scroll_process(self):
        if self.gstate.SCROLL_UP:
            self.camera_y = min(0, self.camera_y + 20)
        if self.gstate.SCROLL_DOWN:
            self.camera_y = max(self.window_size[1] - self.canvas_size[1], self.camera_y - 20)
        if self.gstate.SCROLL_LEFT:
            self.camera_x = min(0, self.camera_x + 20)
        if self.gstate.SCROLL_RIGHT:
            self.camera_x = max(self.window_size[0] - self.canvas_size[0], self.camera_x - 20)

    def left_click(self, x, y):

        x = max(0, min(self.Map.width - 1, x))
        y = max(0, min(self.Map.height - 1, y))

        # Propagate first to Units
        unit_id = self.game.data['unit'][x][y]

        if unit_id != 0:
            # Unit selection
            unit = self.game.units[unit_id]
            self.gstate.selected_unit = unit
            self.player.left_click(x, y, unit_id)
            return
        else:
            # Click on tile
            self.gstate.selected_unit = None

            pass

    def right_click(self, x, y):

        x = max(0, min(self.Map.width - 1, x))
        y = max(0, min(self.Map.height - 1, y))

        # Right click and hav a selected unit, move it!
        if self.gstate.selected_unit:
            self.player.right_click(x, y, self.gstate.selected_unit.unit_id)



    def process(self):
        for event in pygame.event.get():

            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == Input.LEFT:
                x, y = event.pos
                x += self.camera_x * -1
                y += self.camera_y * -1
                tile_x = int(x / Map.TILE_SIZE)
                tile_y = int(y / Map.TILE_SIZE)
                self.left_click(tile_x, tile_y)

            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == Input.RIGHT:
                x, y = event.pos
                x += self.camera_x * -1
                y += self.camera_y * -1
                tile_x = int(x / Map.TILE_SIZE)
                tile_y = int(y / Map.TILE_SIZE)
                self.right_click(tile_x, tile_y)
            elif (event.type == KEYDOWN):
                if (event.key == K_w):
                    self.gstate.SCROLL_UP = True
                elif (event.key == K_a):
                    self.gstate.SCROLL_LEFT = True
                elif (event.key == K_s):
                    self.gstate.SCROLL_DOWN = True
                elif (event.key == K_d):
                    self.gstate.SCROLL_RIGHT = True
            elif event.type == KEYUP:
                if event.key == K_w:
                    self.gstate.SCROLL_UP = False
                elif event.key == K_a:
                    self.gstate.SCROLL_LEFT = False
                elif event.key == K_s:
                    self.gstate.SCROLL_DOWN = False
                elif event.key == K_d:
                    self.gstate.SCROLL_RIGHT = False

                # Hotkeys for building
                if self.gstate.selected_unit:
                    num_options = len(self.gstate.selected_unit.buildable)
                    keys = [K_1, K_2, K_3, K_4, K_5, K_6]
                    for idx, key in enumerate(keys[:num_options]):
                        if event.key == key:
                            if self.gstate.selected_unit.build(idx):
                                pass #self.gstate.selected_unit = None




        # Processes
        self.scroll_process()

    def render(self, dt):
        self.process()

        # Draw Tile map
        self.render_tiles()
        self.render_units(dt)

        self.display.blit(self.canvas, (self.camera_x, self.camera_y))


        Overlay.render_top_panel(self)
        Overlay.render_bottom_panel(self)

        #pygame.display.flip()
        pygame.display.update()

    def caption(self, dt):
        pygame.display.set_caption(
            ' '.join(('WarC2Sim', '[FPS=%d UPS=%d MUL=x%d]')) % (
                self.game.clock.fps,
                self.game.clock.ups,
                (self.game.clock.ups / Config.FRAME_MULTIPLIER)))


    def reset(self):
        self.gstate.selected_unit = None