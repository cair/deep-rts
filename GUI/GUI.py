import pygame
import sys
import numpy as np
import random
import os
from Mechanics.Map import Map
from Mechanics.Unit.Barracks import Barracks
from Mechanics.Unit.Farm import Farm
from Mechanics.Unit.Footman import Footman
from Mechanics.Unit.Peasant import Peasant
from Mechanics.Unit.TownHall import TownHall
from Mechanics.Util import SpriteUtil
from Mechanics.Constants import Unit, Config
from Mechanics.Constants import Mouse as MouseC
from Mechanics.Constants import Map as MapC
from pygame.locals import *
import itertools

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


class GUISelectionPanel:

    @staticmethod
    def top_panel(gui):
        font = pygame.font.SysFont("monospace", 15)

        # Panel background
        pygame.draw.rect(
            gui.display,
            pygame.locals.Color('black'),
            (0, 0, gui.display.get_width(), 25)
        )

        lbl = font.render(
            "Lumber %s "
            "Gold: %s "
            "Food %s/%s "
            "Units: %s "
            "Seconds: %s  "
            "Frames: %s"
            % (
                gui.player.lumber,
                gui.player.gold,
                gui.player.consumed_food, gui.player.food,
                len(gui.player.units),
                int(gui.game.clock.elapsed_update / Config.FRAME_MULTIPLIER),
                int(gui.game.clock.elapsed_update)
            ), 1, (255, 255, 0))
        gui.display.blit(lbl, (10, 5))


    @staticmethod
    def unit_panel(gui):
        if not gui.gstate.selected_unit:
            return

        font = pygame.font.SysFont("monospace", 15)
        font_h1 = pygame.font.SysFont("monospace", 24)

        # Border
        pygame.draw.rect(
            gui.display,
            pygame.locals.Color('green'),
            (0, gui.display.get_height() - 102, gui.display.get_width(), 10)
        )
        # Panel background
        pygame.draw.rect(
            gui.display,
            pygame.locals.Color('black'),
            (0, gui.display.get_height() - 100, gui.display.get_width(), 100)
        )

        lbl_unit_name = font_h1.render("%s (%s) - %s" % (gui.gstate.selected_unit.name, gui.gstate.selected_unit.unit_id, Unit.state_name(gui.gstate.selected_unit.get_state())), 1, (255, 255, 0))
        lbl_unit_health = font.render("Health: %s/%s" % (gui.gstate.selected_unit.health, gui.gstate.selected_unit.health_max) , 1, (255, 255, 0))
        lbl_unit_attack = font.render("Damage: %s - %s" % (gui.gstate.selected_unit.damage_min, gui.gstate.selected_unit.damage_max), 1, (255, 255, 0))
        lbl_unit_defence = font.render("Armor: %s" % gui.gstate.selected_unit.armor, 1, (255, 255, 0))

        lbl_unit_is_ground = font.render("Ground Unit: %s" % gui.gstate.selected_unit.ground_unit, 1, (255, 255, 0))
        lbl_unit_is_water = font.render("Water Unit: %s" % gui.gstate.selected_unit.water_unit, 1, (255, 255, 0))
        lbl_unit_speed = font.render("Speed: %s" % gui.gstate.selected_unit.speed, 1, (255, 255, 0))

        lbl_unit_lumber = font.render("Lumber: %s" % gui.gstate.selected_unit.inventory_lumber, 1, (255, 255, 0))
        lbl_unit_gold = font.render("Gold: %s" % gui.gstate.selected_unit.inventory_gold, 1, (255, 255, 0))
        lbl_unit_oil = font.render("Oil: %s" % gui.gstate.selected_unit.inventory_oil, 1, (255, 255, 0))

        gui.display.blit(lbl_unit_name, (250, 500))
        gui.display.blit(lbl_unit_lumber, (50, 530))
        gui.display.blit(lbl_unit_gold, (50, 545))
        gui.display.blit(lbl_unit_health, (200, 530))
        gui.display.blit(lbl_unit_attack, (200, 545))
        gui.display.blit(lbl_unit_defence, (200, 560))
        gui.display.blit(lbl_unit_is_ground, (350, 530))
        gui.display.blit(lbl_unit_is_water, (350, 545))
        gui.display.blit(lbl_unit_speed, (350, 560))

        # Buildings (Can build)
        btn_x = 615
        btn_y = 510
        buttons = []
        for building in gui.gstate.selected_unit.can_build:
            btn = Button(gui, building, btn_x, btn_y + (20 * len(buttons)))
            buttons.append(btn)



class GUI:

    def __init__(self, game, player):
        self.camera_x = 0
        self.camera_y = 0
        self.game = game
        self.player = player
        self.gstate = GUIState()

        # Calculate window size
        self.window_size = (self.game.map.MAP_WIDTH * MapC.TILE_SIZE, self.game.map.MAP_HEIGHT * MapC.TILE_SIZE)

        pygame.init()
        pygame.display.set_caption('WarC2Sim')

        # Canvas's
        self.display = pygame.display.set_mode((800, 600)) # Window
        self.canvas = pygame.Surface(self.window_size)

        self.tiles_sprite = self.tile_sprites()  # tile sprites (images)
        self.unit_sprite = self.unit_sprites()

        self.tiles_drawn = False

        self.set_camera()

    def set_camera(self):
        try:
            player_unit = self.player.units[0]

            pix_x, pix_y = player_unit.x * MapC.TILE_SIZE * -1, player_unit.y * MapC.TILE_SIZE * -1

            self.camera_x = pix_x + (self.window_size[0] / 3)
            self.camera_y = pix_y + (self.window_size[1] / 4)

        except:
            pass

    def tile_sprites(self):
        tileset_path = os.path.abspath(os.path.join('./data/textures', Map.TILES_THEME, "tiles.png"))
        sheet = pygame.image.load(tileset_path).convert()

        result = [[None for x in range(self.game.map.tile_map.shape[0])] for y in range(self.game.map.tile_map.shape[1])]

        # Predefine sprites for each tile
        for (x, y), tile_type in np.ndenumerate(self.game.map.tile_map):
            tile_info = MapC.TILE_DATA[tile_type]
            tile_choice = random.choice(tile_info['id'])
            sprite = SpriteUtil.image_at(sheet, tile_choice, MapC.TILE_SIZE).convert()
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
                ]
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

    def draw_fow(self):
        vision = np.nonzero(self.player.fow == 0)
        for x, y in zip(vision[0], vision[1]):
            pygame.draw.rect(self.canvas, (0, 0, 0), (x * MapC.TILE_SIZE, y * MapC.TILE_SIZE, MapC.TILE_SIZE, Map.TILE_SIZE))

    def draw_tilemap(self):
        for (x, y), elem in np.ndenumerate(self.game.map.tile_map):
            self.canvas.blit(self.tiles_sprite[x][y], (x * MapC.TILE_SIZE, y * MapC.TILE_SIZE))

    def draw_units(self, dt):

        for unit in self.game.units().values():
            sprite_arr = self.unit_sprite[unit.id][unit.direction] # Selection
            sprite = sprite_arr[unit.animation_iterator % len(sprite_arr)] # Selection based on frame

            self.canvas.blit(sprite, (unit.x * MapC.TILE_SIZE, unit.y * MapC.TILE_SIZE))

            unit.animation_timer += dt
            if unit.animation_timer > unit.animation_interval and unit.path:
                unit.animation_iterator += 1
                unit.animation_timer = 0

    def scroll_process(self):
        if self.gstate.SCROLL_UP:
            self.camera_y = min(0, self.camera_y + 20)
        if self.gstate.SCROLL_DOWN:
            self.camera_y -= 20
        if self.gstate.SCROLL_LEFT:
            self.camera_x = min(0, self.camera_x + 20)
        if self.gstate.SCROLL_RIGHT:
            self.camera_x -= 20

    def left_click(self, t_x, t_y):

        # Propagate first to Units
        try:
            unit_id = self.game.unit_map[t_x][t_y]
        except:
            return

        if unit_id != 0:
            # Unit selection
            unit = self.game.units[unit_id]
            self.gstate.selected_unit = unit
            return
        else:
            # Click on tile
            self.gstate.selected_unit = None

            pass

    def right_click(self, t_x, t_y):

        # Right click and hav a selected unit, move it!
        if self.gstate.selected_unit:
            self.gstate.selected_unit.right_click(t_x, t_y)



    def process(self):
        for event in pygame.event.get():

            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == MouseC.LEFT:
                x, y = event.pos
                x += self.camera_x * -1
                y += self.camera_y * -1
                tile_x = int(x / MapC.TILE_SIZE)
                tile_y = int(y / MapC.TILE_SIZE)
                self.left_click(tile_x, tile_y)

            elif event.type == pygame.MOUSEBUTTONDOWN and event.button == MouseC.RIGHT:
                x, y = event.pos
                x += self.camera_x * -1
                y += self.camera_y * -1
                tile_x = int(x / MapC.TILE_SIZE)
                tile_y = int(y / MapC.TILE_SIZE)
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
                    num_options = len(self.gstate.selected_unit.can_build)
                    keys = [K_1, K_2, K_3, K_4, K_5, K_6]
                    for idx, key in enumerate(keys[:num_options]):
                        if event.key == key:
                            if self.gstate.selected_unit.build(idx):
                                pass #self.gstate.selected_unit = None




        # Processes
        self.scroll_process()

    def draw(self, dt):

                # Draw Tile map
                self.draw_tilemap()
                self.draw_units(dt)
                #self.draw_fow()



                self.display.blit(self.canvas, (self.camera_x, self.camera_y))

                GUISelectionPanel.unit_panel(self)
                GUISelectionPanel.top_panel(self)

                #pygame.display.flip()
                pygame.display.update()

    def caption(self, dt):
        pygame.display.set_caption(
            ' '.join(('Loop=GameClock Tab:[TPS=%d MaxFPS=%d]',
                      'Runtime:[FPS=%d UPS=%d]')) % (
                self.game.clock.max_ups,
                self.game.clock.max_fps,
                self.game.clock.fps,
                self.game.clock.ups))

