from abc import abstractmethod

from DeepRTS.Engine import Map, UnitManager, Constants, Player
from DeepRTS.Engine.Constants import Unit, Direction, Tile, State
from DeepRTS.python import util


import contextlib
with contextlib.redirect_stdout(None):
    import pygame

import os
SURFTYPE = pygame.SWSURFACE #pygame.SWSURFACE | pygame.ASYNCBLIT  | pygame.DOUBLEBUF

dir_path = os.path.dirname(os.path.realpath(__file__))


class SpriteLoader:

    def __init__(self, base_bath):
        self.base_path = base_bath


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


class AbstractGUI:

    def __init__(self, game, tile_size, units, config, window=True):
        self.game = game
        self.config = config
        self.tile_size = tile_size

        self.map = self.game.map
        self.has_window = window
        self.game_height = self.map.map_height * tile_size
        self.game_width = self.map.map_width * tile_size
        self.map_height = self.map.map_height
        self.map_width = self.map.map_width

        self.rect_manager = RectangleManager(self.map_height, self.map_width, self.tile_size)
        self.rect_manager.full_refresh()

        self.mask = {
            0: (0, 0, 255),  # Player 1
            1: (255, 0, 0),
            2: (0, 255, 0),
            3: (255, 255, 0),
            4: (0, 255, 255),
            5: (255, 0, 255),  # Player 6,
            100: (255, 255, 255)
        }

        self.units = units
        self.directions = {
            1: "Down",
            2: "Up",
            3: "Left",
            4: "Right",
            5: "DownLeft",
            6: "DownRight",
            7: "UpLeft",
            8: "UpRight"
        }
        # TODO fastgui/simplegui

        ######################################
        # Canvas initialisation
        ######################################
        pygame.display.init()
        pygame.display.set_caption("DeepRTS v2.5")
        if self.has_window:
            self.canvas = pygame.display.set_mode((self.game_height, self.game_width))
        else:
            self.canvas = pygame.Surface((self.game_height, self.game_width), flags=SURFTYPE)

        # Audio
        if self.config.audio:
            pygame.mixer.init()

            sound = pygame.mixer.Sound(os.path.join(dir_path, "assets", "audio", "song_1.ogg"))
            sound.set_volume(self.config.audio_volume)
            sound.play(loops=-1)

        self.setup()

    def setup(self):
        ######################################
        # Sprite initialisation
        ######################################
        self.sprite_units = self._load_sprites(os.path.join(dir_path, "assets"))
        self.sprite_tiles = self._load_tiles(os.path.join(dir_path, "assets", "textures", "tiles.png"))
        self.health_bars = self._generate_health_bars()

    def _generate_health_bars(self):
        health_bars = {}

        for i, size in enumerate([32, 64, 96, 128]):

            health_bars[i+1] = []

            bar_height = 2
            bar_width = size
            for h in range(0, 101):
                health_bar_surface = pygame.Surface((bar_width, bar_height), flags=SURFTYPE).convert_alpha()

                rect_green = pygame.Rect((0, 0, h, bar_height))  # Green
                rect_red = pygame.Rect((h, 0, bar_width - h, bar_height))  # red

                health_bar_surface.fill((0, 255, 0, 80), rect=rect_green)
                health_bar_surface.fill((255, 0, 0, 80), rect=rect_red)

                health_bars[i+1].append(health_bar_surface)

        return health_bars

    @abstractmethod
    def sprite_definitions(self):
        raise NotImplementedError("This is a abstract method and must be implemented!")

    @abstractmethod
    def tile_definitions(self):
        raise NotImplementedError("This is a abstract method and must be implemented!")

    def on_tile_change(self, tile):
        self.rect_manager.add_changed_tile(tile)
        self.render(changes_only=True)

    def event(self):
        ev = pygame.event.get()

        for event in ev:

            if event.type == pygame.MOUSEBUTTONDOWN:
                abs_pos = pygame.mouse.get_pos()
                tile_pos_x = int(abs_pos[0] / self.tile_size)
                tile_pos_y = int(abs_pos[1] / self.tile_size)

                if event.button == pygame.BUTTON_LEFT:
                    self.game.selected_player.left_click(tile_pos_x, tile_pos_y)

                elif event.button == pygame.BUTTON_RIGHT:
                    self.game.selected_player.right_click(tile_pos_x, tile_pos_y)

            if event.type == pygame.KEYDOWN and self.game.selected_player:
                targeted_unit = self.game.selected_player.get_targeted_unit()

                if targeted_unit:

                    if event.key == pygame.K_1:
                        # Build town-hall
                        targeted_unit.build(0)

                    elif event.key == pygame.K_2:
                        # Build barracks
                        targeted_unit.build(1)

                    elif event.key == pygame.K_3:
                        # Build unit 1
                        targeted_unit.build(2)



    def draw_tile(self, tile, rect):
        tile_type = tile.get_type_id()
        sprite = self.sprite_tiles[tile_type]
        self.canvas.blit(sprite, rect)

    def draw_unit(self, tile, rect):

        unit = self.game.units[tile.get_occupant_id()]
        if unit.tile and unit.tile.id != tile.id or (not unit.structure and unit.state.id == Constants.State.Spawning):
            return False

        # Draw unit
        self.draw_tile(tile, rect)

        # TODO make more flexible
        # Now we are constrained to -> [Unit.id][Direction][State]
        unit_type = unit.type
        direction = unit.direction
        state_id = unit.state.id
        player_id = unit.get_player().get_id()  # TODO - prolly slow?

        try:
            v = self.sprite_units[player_id][unit_type][direction][state_id]
        except KeyError:
            err = "Could not find appropriate sprite for %s:%s:%s" % (unit_type, self.directions[direction], state_id)
            raise RuntimeError(err)

        #print(unit_type, self.directions[direction], state_id)
        unit_rect = [rect.x, rect.y, rect.width * unit.width, rect.height * unit.height]
        self.rect_manager.add_changed_rect(unit_rect)

        if self.config.unit_animation:

            if unit.state.id == Constants.State.Walking:
                interval = unit.walking_interval
                timer = unit.walking_timer
            elif unit.state.id == Constants.State.Harvesting:
                interval = unit.harvest_interval
                timer = unit.harvest_timer
            elif unit.state.id == Constants.State.Combat:
                interval = unit.combat_interval
                timer = unit.combat_timer
            else:
                interval = 1
                timer = 1

            # Animation and sprite rendering
            animation_progress = int((timer / interval) * (len(v) - 1))
            selected_sprite = v[animation_progress]
        else:
            selected_sprite = v[0]

        self.canvas.blit(selected_sprite, unit_rect)

        # Health rendering
        if self.config.unit_health:
            health_percent = int((unit.health / unit.health_max) * 100)
            health_surface = self.health_bars[unit.width][health_percent]

            self.canvas.blit(health_surface, unit_rect)

        return True

    def render(self, changes_only=False):
        if not self.config.render:
            return

        if not self.config.unit_animation:
            changes_only = True

        updated_tiles = self.rect_manager.changed_tiles
        if not changes_only:
            updated_tiles += [unit.tile for unit in self.game.units if unit.tile]

        for tile in updated_tiles:
            i = tile.y * self.map_height + tile.x
            rect = self.rect_manager.rectangles[i]
            self.rect_manager.add_changed_rect(rect)

            # Draw tile
            if tile.has_occupant():

                self.draw_unit(tile, rect)
            else:
                self.draw_tile(tile, rect)

            # No rect was generated by unit (these can potentially be bigger than standard 1x tile
            # hence, we add a larger rect if a unit is drawn at the end of draw_unit function)

        self.rect_manager.clear_changed_tile()


        pass

    def view(self):
        if not self.has_window:
            return

        pygame.display.update(self.rect_manager.changed_rects)
        #pygame.display.flip()
        self.rect_manager.clear_changed_rect()

    def _load_sprites(self, path):
        sprite_def = self.sprite_definitions()

        loaded_sprites = {}

        for player_id, mask in self.mask.items():

            loaded_sprites[player_id] = {}

            for unit_type, directions in sprite_def.items():
                file_name = directions["file_name"]
                sheet = pygame.image.load(open(os.path.join(path, file_name), "r"))
                unit = self.units[unit_type]

                loaded_sprites[player_id][unit_type] = {}

                for direction, defs in directions.items():
                    if direction == "file_name":
                        continue

                    loaded_sprites[player_id][unit_type][direction] = {}

                    for state, values in defs.items():
                        loaded_sprites[player_id][unit_type][direction][state] = []

                        for (x, y, flip) in values:
                            sprite = util.get_sprite(sheet, x, y, self.tile_size * unit.height, flip)

                            # Tinting
                            self._color_surface(sprite, (170, 170, 170), mask)

                            # Border
                            if self.config.unit_outline:
                                pygame.draw.rect(sprite, mask, (0, 0, sprite.get_width(), sprite.get_height()), 2)

                            loaded_sprites[player_id][unit_type][direction][state].append(sprite)





        return loaded_sprites

    def _load_tiles(self, path):
        tile_ids, size = self.tile_definitions()
        sheet = pygame.image.load(path)

        tiles = {
            tile: util.image_at(sheet, tile - 1, size).convert_alpha()for tile in tile_ids
        }

        return tiles

    def _color_surface(self, surface, original, newcolor):
        arr = pygame.surfarray.pixels3d(surface)

        r1, g1, b1 = original
        r2, g2, b2 = newcolor
        red, green, blue = arr[:, :, 0], arr[:, :, 1], arr[:, :, 2]
        mask = (red >= r1) & (green >= g1) & (blue >= b1)
        arr[:, :, :3][mask] = [r2, g2, b2]


class GUI(AbstractGUI):

    def __init__(self, game, tile_size, config):
        possible_unit_types = [
            getattr(Unit, x) for x in Unit.__dict__.keys() if not x.startswith("__") and not x == 'name'
        ]

        arb_player = Player(game, -1)  # Create arbitrary player object to create unit manager

        units = {
            x: UnitManager.construct_unit(x, arb_player)
            for x in possible_unit_types
        }

        super().__init__(game, tile_size, units, config)

    def tile_definitions(self):
        tile_types = [int(getattr(Tile, x)) for x in Tile.__dict__.keys() if not x.startswith("__") and not x == 'name']
        return tile_types, self.tile_size

    def sprite_definitions(self):
        return {
            Unit.Peasant: {
                "file_name": "textures/peasant.png",
                Direction.Up: {
                    State.Walking: [(0, 0, False), (32, 0, False), (64, 0, False), (96, 0, False), (128, 0, False)],
                    State.Idle: [(0, 0, False)],

                    State.Building: [(0, 288, False), (32, 288, False), (64, 288, False), (96, 288, False), (128, 288, False)],
                    State.Harvesting: [(0, 288, False), (32, 288, False), (64, 288, False), (96, 288, False), (128, 288, False)],
                    State.Combat: [(0, 288, False), (32, 288, False), (64, 288, False), (96, 288, False), (128, 288, False)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.Down: {
                    State.Walking: [(0, 32, False), (32, 32, False), (64, 32, False), (96, 32, False),(128, 32, False)],
                    State.Idle: [(0, 32, False)],

                    State.Building: [(0, 352, False), (32, 352, False), (64, 352, False), (96, 352, False), (128, 352, False)],
                    State.Harvesting: [(0, 352, False), (32, 352, False), (64, 352, False), (96, 352, False), (128, 352, False)],
                    State.Combat: [(0, 352, False), (32, 352, False), (64, 352, False), (96, 352, False), (128, 352, False)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.Right: {
                    State.Walking: [(0, 64, False), (32, 64, False), (64, 64, False), (96, 64, False),(128, 64, False)],
                    State.Idle: [(0, 64, False)],

                    State.Building: [(0, 320, False), (32, 320, False), (64, 320, False), (96, 320, False), (128, 320, False)],
                    State.Harvesting: [(0, 320, False), (32, 320, False), (64, 320, False), (96, 320, False)],
                    State.Combat: [(0, 320, False), (32, 320, False), (64, 320, False), (96, 320, False)],


                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.Left: {
                    State.Walking: [(0, 96, False), (32, 96, False), (64, 96, False), (96, 96, False),(128, 96, False)],
                    State.Idle: [(0, 96, False)],

                    State.Building: [(0, 320, True), (32, 320, True), (64, 320, True), (96, 320, True), (128, 320, True)],
                    State.Harvesting: [(0, 320, True), (32, 320, True), (64, 320, True), (96, 320, True)],
                    State.Combat:  [(0, 320, True), (32, 320, True), (64, 320, True), (96, 320, True)],


                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.UpRight: {
                    State.Walking: [(0, 128, False), (32, 128, False), (64, 128, False), (96, 128, False),(128, 128, False)],
                    State.Idle: [(0, 128, False)],

                    State.Building: [(0, 256, False), (32, 256, False), (64, 256, False)],
                    State.Harvesting: [(0, 256, False), (32, 256, False), (64, 256, False)],
                    State.Combat: [(0, 256, False), (32, 256, False), (64, 256, False)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.UpLeft: {
                    State.Walking: [(0, 160, False), (32, 160, False), (64, 160, False), (96, 160, False), (128, 160, False)],
                    State.Idle: [(0, 160, False)],

                    State.Building: [(0, 256, True), (32, 256, True), (64, 256, True)],
                    State.Harvesting: [(0, 256, True), (32, 256, True), (64, 256, True)],
                    State.Combat: [(0, 256, True), (32, 256, True), (64, 256, True)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.DownRight: {
                    State.Walking: [(0, 192, False), (32, 192, False), (64, 192, False), (96, 192, False),(128, 192, False)],
                    State.Idle: [(0, 192, False)],

                    State.Building: [(0, 320, False), (32, 320, False)],
                    State.Harvesting: [(32, 320, False), (64, 352, False), (96, 352, False),  (128, 352, False)],
                    State.Combat: [(32, 320, False), (64, 352, False), (96, 352, False),  (128, 352, False)],


                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.DownLeft: {
                    State.Walking: [(0, 224, False), (32, 224, False), (64, 224, False), (96, 224, False),(128, 224, False)],
                    State.Idle: [(0, 224, False)],

                    State.Building: [(0, 320, True), (32, 320, True)],
                    State.Harvesting: [(32, 320, True), (64, 352, True), (96, 352, True), (128, 352, True)],
                    State.Combat: [(32, 320, True), (64, 352, True), (96, 352, True), (128, 352, True)],


                    State.Dead: [(164, 4, False)]  # TODO
                },
            },
            Unit.Footman: {
                "file_name": "textures/footman.png",
                Direction.Up: {
                    State.Walking: [(0, 0, False), (32, 0, False), (64, 0, False), (96, 0, False), (128, 0, False)],
                    State.Combat: [(0, 160, False), (32, 160, False), (64, 160, False), (96, 160, False)],
                    State.Idle: [(0, 0, False)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.Down: {
                    State.Walking: [(0, 128, False), (32, 128, False), (64, 128, False), (96, 128, False), (128, 128, False)],
                    State.Combat: [(0, 288, False), (32, 288, False), (64, 288, False), (96, 288, False)],
                    State.Idle: [(0, 128, False)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.Right: {
                    State.Walking: [(0, 64, False), (32, 64, False), (64, 64, False), (96, 64, False), (128, 64, False)],
                    State.Combat: [(0, 224, False), (32, 224, False), (64, 224, False), (96, 224, False)],
                    State.Idle: [(0, 64, False)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.Left: {
                    State.Walking: [(0, 64, True), (32, 64, True), (64, 64, True), (96, 64, True), (128, 64, True)],
                    State.Combat: [(0, 224, True), (32, 224, True), (64, 224, True), (96, 224, True)],
                    State.Idle: [(0, 64, True)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.UpRight: {
                    State.Walking: [(0, 32, False), (32, 32, False), (64, 32, False), (96, 32, False), (128, 32, False)],
                    State.Combat: [(0, 192, False), (32, 192, False), (64, 192, False)],
                    State.Idle: [(0, 32, False)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.UpLeft: {
                    State.Walking: [(0, 32, True), (32, 32, True), (64, 32, True), (96, 32, True), (128, 32, True)],
                    State.Combat: [(0, 192, True), (32, 192, True), (64, 192, True)],
                    State.Idle: [(0, 32, True)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.DownRight: {
                    State.Walking: [(0, 96, False), (32, 96, False), (64, 96, False), (96, 96, False), (128, 96, False)],
                    State.Combat: [(0, 256, False), (32, 256, False), (64, 256, False), (96, 256, False)],
                    State.Idle: [(0, 96, False)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.DownLeft: {
                    State.Walking: [(0, 96, True), (32, 96, True), (64, 96, True), (96, 96, True), (128, 96, True)],
                    State.Combat: [(0, 256, True), (32, 256, True), (64, 256, True), (96, 256, True)],
                    State.Idle: [(0, 96, True)],

                    State.Dead: [(164, 4, False)]  # TODO
                },
            },
            Unit.Archer: {
                "file_name": "textures/archer.png",
                Direction.Up: {
                    State.Walking: [(0, 32, False), (32, 32, False), (64, 32, False), (96, 32, False)],
                    State.Combat: [(0, 192, False), (32, 192, False)],

                    State.Idle: [(0, 32, False)],
                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.Down: {
                    State.Walking: [(0, 160, False), (32, 160, False), (64, 160, False), (96, 160, False),
                                    (128, 160, False)],
                    State.Combat: [(0, 320, False), (32, 320, False)],

                    State.Idle: [(0, 160, False)],
                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.Right: {
                    State.Walking: [(0, 96, False), (32, 96, False), (64, 96, False), (96, 96, False)],
                    State.Combat: [(0, 256, False), (32, 256, False)],

                    State.Idle: [(0, 96, False)],
                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.Left: {
                    State.Walking: [(0, 96, True), (32, 96, True), (64, 96, True), (96, 96, True)],
                    State.Combat: [(0, 256, True), (32, 256, True)],

                    State.Idle: [(0, 96, True)],
                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.UpRight: {
                    State.Walking: [(0, 32, False), (32, 32, False), (64, 32, False), (96, 32, False),
                                    (128, 32, False)],
                    State.Combat: [(0, 224, False), (32, 224, False)],

                    State.Idle: [(0, 32, False)],
                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.UpLeft: {
                    State.Walking: [(0, 32, True), (32, 32, True), (64, 32, True), (96, 32, True), (128, 32, True)],
                    State.Combat: [(0, 224, True), (32, 224, True)],

                    State.Idle: [(0, 32, True)],
                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.DownRight: {
                    State.Walking: [(0, 128, False), (32, 128, False), (64, 128, False), (96, 128, False),
                                    (128, 128, False)],
                    State.Combat: [(0, 288, False), (32, 288, False)],

                    State.Idle: [(0, 128, False)],
                    State.Dead: [(164, 4, False)]  # TODO
                },
                Direction.DownLeft: {
                    State.Walking: [(0, 128, True), (32, 128, True), (64, 128, True), (96, 128, True), (128, 128, True)],
                    State.Combat: [(0, 288, True), (32, 288, True)],

                    State.Idle: [(0, 128, True)],
                    State.Dead: [(164, 4, False)]  # TODO
                },
            },
            Unit.Barracks: {
                "file_name": "textures/barracks.png",
                Direction.Down: {
                    State.Spawning: [(96, 0, False)],
                    State.Idle: [(0, 0, False)],
                    State.Building: [(0, 0, False)],
                    State.Dead: [(96, 0, False)]
                },
            },
            Unit.TownHall: {
                "file_name": "textures/town_hall.png",
                Direction.Down: {
                    State.Spawning: [(96, 0, False)],
                    State.Idle: [(0, 0, False)],
                    State.Building: [(0, 0, False)],
                    State.Dead: [(96, 0, False)]
                },
            },
            Unit.Farm: {
                "file_name": "textures/farm.png",
                Direction.Down: {
                    State.Spawning: [(32, 0, False)],
                    State.Idle: [(0, 0, False)],
                    State.Dead: [(32, 0, False)]
                },

            }
        }


