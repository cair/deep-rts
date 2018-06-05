import pygame
from util import get_sprite, image_at
from pyDeepRTS.Constants import Unit, Direction, Tile
import os
dir_path = os.path.dirname(os.path.realpath(__file__))

class Sprites:

    def __init__(self, gui):
        self.gui = gui
        self.simple = False#self.gui.game.simple

        self.mask = {
            0: (255, 0, 0),  # Player 1
            1: (0, 0, 255),
            2: (0, 255, 0),
            3: (255, 255, 0),
            4: (0, 255, 255),
            5: (255, 0, 255)  # Player 6
        }

        self.peasant = self._load_sprite("assets/textures/human/peasant.png")
        self.archer = self._load_sprite("assets/textures/human/archer.png")
        self.footman = self._load_sprite("assets/textures/human/footman.png")
        buildings = self._load_sprite("assets/textures/human/buildings.png")
        self.town_hall = buildings
        self.barracks = buildings
        self.farm = buildings

        if self.simple:
            self.peasant = pygame.Surface((self.peasant.get_width(), self.peasant.get_height()))
            self.archer = pygame.Surface((self.archer.get_width(), self.archer.get_height()))
            self.footman = pygame.Surface((self.footman.get_width(), self.footman.get_height()))
            self.town_hall = pygame.Surface((buildings.get_width(), buildings.get_height()))
            self.barracks = pygame.Surface((buildings.get_width(), buildings.get_height()))
            self.farm = pygame.Surface((buildings.get_width(), buildings.get_height()))

            self.peasant.fill((255, 0, 0))
            self.archer.fill((255, 255, 0))
            self.footman.fill((0, 255, 255))

            self.town_hall.fill((0, 255, 255))
            self.barracks.fill((255, 0, 255))
            self.farm.fill((0, 255, 0))


    def load(self):
        return self.sprites(), self.tiles()

    def _load_sprite(self, path):
        try:
            full_path = os.path.join(dir_path,  path)
            sheet = pygame.image.load(full_path).convert_alpha()
        except pygame.error:
            full_path = os.path.join(dir_path,  path)
            sheet = pygame.image.load(full_path).convert_alpha()


        return sheet

    def tiles(self):
        try:
            tileset_path = os.path.join(dir_path, "assets", "textures", "tiles.png")
            sheet = pygame.image.load(tileset_path)
        except pygame.error:
            tileset_path = os.path.join(dir_path,  "assets", "textures", "tiles.png")
            sheet = pygame.image.load(tileset_path)

        tile_types = [int(getattr(Tile, x)) for x in Tile.__dict__.keys() if not x.startswith("__") and not x == 'name']

        tiles = {}
        for tile_type in tile_types:
            sprite = image_at(sheet, tile_type - 1, self.gui.map.tile_width).convert_alpha()  # -1 to account for index 0
            tiles[tile_type] = sprite

        return tiles

    def sprites(self):
        sprites = {
            Unit.Peasant:  {
                Direction.Left: [
                    get_sprite(self.peasant, 85, 4, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 85, 42, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 85, 82, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 85, 118, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 85, 155, 32, 32, 1, 1, True)
                ],
                Direction.Up: [
                    get_sprite(self.peasant, 15, 4, 32, 32, 1, 1),
                    get_sprite(self.peasant, 15, 42, 32, 32, 1, 1),
                    get_sprite(self.peasant, 15, 82, 32, 32, 1, 1),
                    get_sprite(self.peasant, 15, 118, 32, 32, 1, 1),
                    get_sprite(self.peasant, 15, 155, 32, 32, 1, 1)
                ],
                Direction.Right: [
                    get_sprite(self.peasant, 85, 4, 32, 32, 1, 1),
                    get_sprite(self.peasant, 85, 42, 32, 32, 1, 1),
                    get_sprite(self.peasant, 85, 82, 32, 32, 1, 1),
                    get_sprite(self.peasant, 85, 118, 32, 32, 1, 1),
                    get_sprite(self.peasant, 85, 155, 32, 32, 1, 1)
                ],
                Direction.Down: [
                    get_sprite(self.peasant, 164, 4, 32, 32, 1, 1),
                    get_sprite(self.peasant, 164, 42, 32, 32, 1, 1),
                    get_sprite(self.peasant, 164, 82, 32, 32, 1, 1),
                    get_sprite(self.peasant, 164, 118, 32, 32, 1, 1),
                    get_sprite(self.peasant, 164, 155, 32, 32, 1, 1)
                ],
                Direction.UpLeft: [
                    get_sprite(self.peasant, 50, 4, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 50, 42, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 50, 82, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 50, 118, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 50, 155, 32, 32, 1, 1, True)
                ],
                Direction.UpRight: [
                    get_sprite(self.peasant, 50, 4, 32, 32, 1, 1),
                    get_sprite(self.peasant, 50, 42, 32, 32, 1, 1),
                    get_sprite(self.peasant, 50, 82, 32, 32, 1, 1),
                    get_sprite(self.peasant, 50, 118, 32, 32, 1, 1),
                    get_sprite(self.peasant, 50, 155, 32, 32, 1, 1)
                ],
                Direction.DownLeft: [
                    get_sprite(self.peasant, 120, 4, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 120, 42, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 120, 82, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 120, 118, 32, 32, 1, 1, True),
                    get_sprite(self.peasant, 120, 155, 32, 32, 1, 1, True)
                ],
                Direction.DownRight: [
                    get_sprite(self.peasant, 120, 4, 32, 32, 1, 1),
                    get_sprite(self.peasant, 120, 42, 32, 32, 1, 1),
                    get_sprite(self.peasant, 120, 82, 32, 32, 1, 1),
                    get_sprite(self.peasant, 120, 118, 32, 32, 1, 1),
                    get_sprite(self.peasant, 120, 155, 32, 32, 1, 1)
                ]
            },
            Unit.Footman:  {
                Direction.Left: [
                    get_sprite(self.footman, 170, 7, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 170, 56, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 170, 99, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 170, 138, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 170, 176, 32, 32, 1, 1, True)
                ],
                Direction.Up: [
                    get_sprite(self.footman, 21, 7, 32, 32, 1, 1),
                    get_sprite(self.footman, 21, 56, 32, 32, 1, 1),
                    get_sprite(self.footman, 21, 99, 32, 32, 1, 1),
                    get_sprite(self.footman, 21, 138, 32, 32, 1, 1),
                    get_sprite(self.footman, 21, 176, 32, 32, 1, 1)
                ],
                Direction.Right: [
                    get_sprite(self.footman, 170, 7, 32, 32, 1, 1),
                    get_sprite(self.footman, 170, 56, 32, 32, 1, 1),
                    get_sprite(self.footman, 170, 99, 32, 32, 1, 1),
                    get_sprite(self.footman, 170, 138, 32, 32, 1, 1),
                    get_sprite(self.footman, 170, 176, 32, 32, 1, 1)
                ],
                Direction.Down: [
                    get_sprite(self.footman, 315, 7, 32, 32, 1, 1),
                    get_sprite(self.footman, 315, 56, 32, 36, 1, 1),
                    get_sprite(self.footman, 315, 99, 32, 32, 1, 1),
                    get_sprite(self.footman, 315, 138, 32, 36, 1, 1),
                    get_sprite(self.footman, 315, 176, 32, 32, 1, 1)
                ],
                Direction.UpLeft: [
                    get_sprite(self.footman, 96, 7, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 96, 56, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 96, 99, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 96, 138, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 96, 176, 32, 32, 1, 1, True)
                ],
                Direction.UpRight: [
                    get_sprite(self.footman, 96, 7, 32, 32, 1, 1),
                    get_sprite(self.footman, 96, 56, 32, 32, 1, 1),
                    get_sprite(self.footman, 96, 99, 32, 32, 1, 1),
                    get_sprite(self.footman, 96, 138, 32, 32, 1, 1),
                    get_sprite(self.footman, 96, 176, 32, 32, 1, 1)
                ],
                Direction.DownLeft: [
                    get_sprite(self.footman, 241, 7, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 241, 56, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 241, 99, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 241, 138, 32, 32, 1, 1, True),
                    get_sprite(self.footman, 241, 176, 32, 32, 1, 1, True)
                ],
                Direction.DownRight: [
                    get_sprite(self.footman, 241, 7, 32, 32, 1, 1),
                    get_sprite(self.footman, 241, 56, 32, 32, 1, 1),
                    get_sprite(self.footman, 241, 99, 32, 32, 1, 1),
                    get_sprite(self.footman, 241, 138, 32, 32, 1, 1),
                    get_sprite(self.footman, 241, 176, 32, 32, 1, 1)
                ],

            },
            Unit.TownHall:  {
                Direction.UpLeft: [
                    get_sprite(self.town_hall, 270, 156, 111, 93, 3, 3),
                ],
                Direction.UpRight: [
                    get_sprite(self.town_hall, 270, 156, 111, 93, 3, 3),
                ],
                Direction.Up: [
                    get_sprite(self.town_hall, 270, 20, 111, 93, 3, 3)
                ]
            },
            Unit.Farm:  {
                Direction.UpLeft: [
                    get_sprite(self.farm, 398, 70, 66, 66, 2, 2)
                ],
                Direction.UpRight: [
                    get_sprite(self.farm, 398, 70, 66, 66, 2, 2)
                ],
                Direction.Up: [
                    get_sprite(self.farm, 398, 1, 66, 66, 2, 2),
                ]
            },
            Unit.Barracks:  {
                Direction.UpLeft: [
                    get_sprite(self.barracks, 304, 560, 102, 102, 3, 3)
                ],
                Direction.UpRight: [
                    get_sprite(self.barracks, 304, 560, 102, 102, 3, 3)
                ],
                Direction.Up: [
                    get_sprite(self.barracks, 304, 457, 102, 102, 3, 3),
                ]
            }
        }

        p_sprites = {}
        for player_id, mask in self.mask.items():
            p_sprites[player_id] = {}
            for unit_type, directions in sprites.items():
                p_sprites[player_id][unit_type] = {}
                for direction, surfaces in directions.items():
                    p_sprites[player_id][unit_type][direction] = []
                    for surface in surfaces:
                        c_surface = surface.copy()
                        self.color_surface(c_surface, (170, 170, 170), mask)

                        # Draw outline
                        pygame.draw.rect(c_surface, mask, (0, 0, c_surface.get_width(), c_surface.get_height()), 5)
                        p_sprites[player_id][unit_type][direction].append(c_surface)
        return p_sprites

    def color_surface(self, surface, original, newcolor):
        arr = pygame.surfarray.pixels3d(surface)

        r1, g1, b1 = original
        r2, g2, b2 = newcolor
        red, green, blue = arr[:, :, 0], arr[:, :, 1], arr[:, :, 2]
        mask = (red >= r1) & (green >= g1) & (blue >= b1)
        arr[:, :, :3][mask] = [r2, g2, b2]