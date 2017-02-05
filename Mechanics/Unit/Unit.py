import itertools
import random
from math import *
from Mechanics.API.Event import Event
from Mechanics.Constants import Config
from Mechanics.Unit.States import State
from Mechanics.Util import ArrayUtil
from Pathfinding import a_star_search

"""
#
# Unit Directions
#
"""
LEFT = 0
UP = 1
RIGHT = 2
DOWN = 3
UP_LEFT = 4
UP_RIGHT = 5
DOWN_LEFT = 6
DOWN_RIGHT = 7

"""
#
# Unit Unique IDs
#
"""
NONE = 0
TOWN_HALL = 1
KEEP = 2
CASTLE = 3
FARM = 4
BARRACKS = 5

PEASANT = 20
FOOTMAN = 21

"""
#
# Structure Types
#
"""
UNIT = 0
STRUCTURE = 1


class UnitState:

    def __init__(self):
        self.player_state = None

        self.x = None
        self.y = None
        self.inventory_lumber = 0
        self.inventory_gold = 0
        self.inventory_oil = 0
        self.health = None
        self.current = None
        self.spawned = False
        
    def transition(self, obj):
        self.current.transition(obj)

    def add_next(self, a, b):
        self.current.add_next(a, b)

    def has_next_state(self, a):
        self.current.has_next_state(a)

    def clear_next(self):
        self.current.clear_next()


class Unit:
    UNIT_COUNTER = 1

    id = -1
    width = 1
    height = 1

    # Animation
    animation_interval = .2 * Config.FRAME_MULTIPLIER
    animation_timer = 0
    animation_iterator = 0

    # Unit Flags
    ground_unit = True
    water_unit = False
    structure = False

    # Attributes
    name = "UNKNOWN"
    damage_min = None
    damage_max = None
    damage_range = None
    damage_piercing = None
    health_max = None
    armor = None
    cost_gold = None
    cost_lumber = None
    inventory_space = 10
    speed = None
    sight = None
    food = 0
    food_cost = 1
    range = 1
    can_harvest = False

    def __init__(self, player, attrs={}):
        self.unit_id = self.generate_id()

        self.tick = 0
        self.direction = DOWN
        self.dimension = self._dimension()

        self.player = player
        self.game = player.game
        self.Map = self.game.Map
        self.data = player.game.state.data
        self.Event = player.Event
        self.state = UnitState()
        self.state.current = State.new(self, State.Despawned)
        self.state.transition(State.new(self, State.Spawning, properties=attrs))

        self.Event.notify(Event.Unit_Spawn, data=self.unit_id)

    def process(self, tick):
        self.tick += tick
        self.state.current.update(tick)

    def unit_area(self):
        map_tiles_x = [x for x in range(self.state.x, self.state.x + self.width)]
        map_tiles_y = [y for y in range(self.state.y, self.state.y + self.height)]
        tiles = list(itertools.product(map_tiles_x, map_tiles_y))
        return tiles

    def set_position(self, x, y):
        prev_x = self.state.x if self.state.x else x
        prev_y = self.state.y if self.state.y else y

        # If unit already have a position
        if self.state.x and self.state.y:
            self.position_matrix(True)
            self.fog(True)

        # Set new position
        self.state.x = x
        self.state.y = y

        # Update Matrix
        self.position_matrix(False)
        self.fog(False)

        # Set direction of step
        self.set_direction(self.state.x - prev_x, self.state.y - prev_y)

        self.Event.notify(Event.Unit_Set_Position, data=(self.unit_id, self.state.x, self.state.y))

    def set_direction(self, dx, dy):
        data = {
            (1, 0): RIGHT,
            (-1, 0): LEFT,
            (0, 1): DOWN,
            (0, 0): DOWN,
            (0, -1): UP,
            (1, 1): DOWN_RIGHT,
            (-1, 1): DOWN_LEFT,
            (-1, -1): UP_LEFT,
            (1, -1): UP_RIGHT
        }
        try:
            self.direction = data[(dx, dy)]
        except:
            self.direction = data[(0, 0)] # TODO!

    def fog(self, reset=False):
        func = self.player.state.vision_map.remove if reset else self.player.state.vision_map.append
        half_sight = floor(self.sight/2)
        y_dim = self.state.y+self.dimension
        x_dim = self.state.x+self.dimension
        for y in self.Map.coord_map[y_dim-half_sight:y_dim+half_sight+1, x_dim-half_sight:x_dim+half_sight+1]:
            for arr in y:
                x = arr[0]
                y = arr[1]
                func((x, y))
                if reset:
                    self.player.vision_memory[x][y][3] = self.player.vision_memory[x][y][2]
                    self.player.vision_memory[x][y][2] = self.player.vision_memory[x][y][1]
                    self.player.vision_memory[x][y][1] = self.player.vision_memory[x][y][0]
                    self.player.vision_memory[x][y][0] = self.game.data['unit'][x][y]

    def position_matrix(self, reset=False):
        i = (self.unit_id, self.player.id) if not reset else (0, 0)

        for x, y in ArrayUtil.get_area(
                self.state.x,
                self.state.y,
                self.width,
                self.height
        ):
            self.data['unit'][x][y] = i[0]
            self.data['unit_pid'][x][y] = i[1]

    def add_to_game(self):
        self.game.units[self.unit_id] = self
        self.player.units.append(self.unit_id)

    def remove_from_game(self):
        del self.game.units[self.unit_id]
        self.player.units.remove(self.unit_id)

    def spawn(self):
        self.position_matrix(False)
        self.fog(False)
        self.player.consumed_food += self.food_cost
        self.player.food += self.food
        self.spawned = True

    def despawn(self):
        self.position_matrix(True)
        self.fog(True)

        self.state.x = None
        self.state.y = None

        self.player.consumed_food -= self.food_cost
        self.player.food -= self.food

        self.spawned = False

    def generate_path(self, x, y):
        path = a_star_search(
            self.game.Map,
            self.game.data['tile'],
            (self.state.x, self.state.y),
            (x, y)
        )

        if path:
            path.pop()

        return path

    def move(self, x, y):
        # Cannot move if building
        if self.structure == STRUCTURE or self.speed <= 0:
            return False

        path = self.generate_path(x, y)

        self.state.current.transition(State.new(self, State.Walking, {
            'path': path,
            'path_goal': (x, y)
        }))

        self.Event.notify(Event.Unit_Move, data=(self.unit_id, x, y))

    def distance(self, x, y, dim=0):
        d = hypot(self.state.x - (x + dim), self.state.y - (y + dim))
        return int(d) - dim

    def _crossover(self, x, y):
        dx1 = self.state.x - x
        dy1 = self.state.y - y

        cross = abs(dx1) + abs(dy1)
        return cross*0.001

    def shortest_distance(self, tiles):
        tiles.sort(key=lambda x: self.distance(x[0], x[1]) + self._crossover(x[0], x[1]), reverse=True)
        return tiles.pop()

    def _dimension(self, clazz = None):

        width = self.width
        height = self.height
        if clazz:
            width = clazz.width
            height = clazz.height

        mid_x = floor(width / 2)
        mid_y = floor(height / 2)

        return int((mid_x + mid_y) / 2)

    def center_tile(self):
        """
        Since self.state.x and self.state.y are the upper left corner,
        on structures which are 3x3 we need to determine center
        :return:
        """
        mid_x = floor(self.width / 2)
        mid_y = floor(self.height / 2)
        return self.state.x - mid_x, self.state.y - mid_y

    def harvest(self, x, y):
        if not self.can_harvest:
            return

        distance = self.distance(x, y)
        if distance > 1:
            tiles = self.game.Map.AdjacentMap.adjacent_walkable(x, y, 1)

            if not tiles:
                return
            tile = self.shortest_distance(tiles)

            if self.state.current.has_next_state(State.Harvesting):
                self.state.current.clear_next()

            self.state.current.add_next(State.new(self, State.Harvesting, {
                'harvest_target': (x, y)
            }), 1)
            self.move(*tile)
        else:
            self.state.current.transition(State.new(self, State.Harvesting, {
                'harvest_target': (x, y)
            }))

    def closest_recall_building(self):
        units = self.game.units
        recall_buildings = [units[x] for x in self.player.units if units[x].id in [
            TOWN_HALL,
            KEEP,
            CASTLE
        ]]

        recall_buildings.sort(key=lambda x: self.distance(x.x, x.y), reverse=True)
        try:
            recall_building = recall_buildings.pop()
        except:
            return None

        return recall_building

    def is_walking(self):
        return self.state.current == self.states[State.Walking]

    def is_worker(self):
        return self.can_harvest

    def get_damage(self, unit):
        # http://classic.battle.net/war2/basic/combat.shtml
        my_damage = (random.randint(self.damage_min, self.damage_max) - unit.armor) + self.damage_piercing
        return random.randint(floor(my_damage * .50), my_damage)

    def afflict_damage(self, damage):
        self.state.health = max(0, self.health - damage)

        if self.state.health <= 0:
            self.state.current.transition(State.new(self, State.Dead))

    def is_dead(self):
        return self.state.type == self.state.Dead

    def attack(self, x, y):

        # Units with no minimum damage cannot attack
        # Warning! This makes unit not move to attack target, if they do not have damage!
        if self.damage_min <= 0:
            return

        attack_target = self.Map.get_unit(x, y)
        distance = self.distance(x, y)
        if distance > 1:

            tiles = self.Map.AdjacentMap.adjacent_walkable(
                attack_target.x + attack_target.dimension,
                attack_target.y + attack_target.dimension,
                attack_target.dimension + 1
            )

            if not tiles:
                return
            tile = self.shortest_distance(tiles)

            if self.state.currenthas_next_state(State.Combat):
                self.state.current.clear_next()

            self.state.current.add_next(State.new(self, State.Combat, {
                'attack_target': attack_target
            }), 1)
            self.move(*tile)
        else:
            self.state.current.transition(State.new(self, State.Combat, {
                'attack_target': attack_target
            }))

    def right_click(self, x, y):
        if not self.state.x or not self.state.y:
            return

        if self.game.Map.is_attackable(self, x, y):
            self.attack(x, y)
        elif not self.structure and self.game.Map.is_harvestable_tile(self, x, y):
            self.harvest(x, y)
        elif self.game.Map.is_walkable_tile(self, x, y):
            self.state.current.clear_next()
            self.move(x, y)

    def can_build_here(self, subject):
        """
        This determines weither this unit can build a unit at current location
        :param subject:
        :param x:
        :param y:
        :return:
        """
        if not self.state.x or not self.state.y:
            return False

        if self.structure:
            adjacent_tiles = self.Map.AdjacentMap.adjacent_walkable(
                self.state.x + self.dimension,
                self.state.y + self.dimension,
                self.dimension + 1
            )
            return len(adjacent_tiles) > 0
        else:
            # A unit builds a structure
            adjacent_tiles = self.Map.AdjacentMap.adjacent_walkable(self.state.x, self.state.y, Unit._dimension(subject))
            adjacent_tiles.append((self.state.x, self.state.y))
            subject_area = list(
                itertools.product(
                    list(range(self.state.x - Unit._dimension(subject), self.state.x - Unit._dimension(subject) + subject.height)),
                    list(range(self.state.y - Unit._dimension(subject), self.state.y - Unit._dimension(subject) + subject.width))
                ))
            common = list(set(adjacent_tiles).intersection(set(subject_area)))
            return len(common) == len(subject_area)

    def build(self, idx):

        entity_class = self.buildable[idx]

        if self.player.consumed_food + entity_class.food_cost > self.player.food:
            print("Not enough food!")
            return False

        if self.player.gold < entity_class.cost_gold or self.player.lumber < entity_class.cost_lumber:
            print("Cannot afford this unit")
            return False

        if self.can_build_here(entity_class):
            # All tests passed, unit can be built
            # Subtract resources
            self.player.gold -= entity_class.cost_gold
            self.player.lumber -= entity_class.cost_lumber

            self.player.statistics['unit_count'] += 1

            self.state.transition(State.new(self, State.Building, {
                'target': entity_class,
            }))

            return True
        else:
            # TODO feedback could not build
            return False
            pass

    @staticmethod
    def generate_id():
        unit_id = Unit.UNIT_COUNTER
        Unit.UNIT_COUNTER += 1
        return unit_id

