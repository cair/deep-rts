import itertools
import random
import math

import game.const.Event as Event
from game.api import Event

from game import Config
from game.const import Unit as UnitC, State
from game.state.Dead import Dead
from game.util import ArrayUtil
from game.util.Pathfinding import a_star_search


class Unit:
    UNIT_COUNTER = 1

    id = UnitC.NONE

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

    def toJSON(self):
        return {
            'id': self.id,
            'state': self.state
        }

    def load(self, data):
        self.id = data['id']

        s_data = data['state']
        state_clazz = State.mapping[s_data['type']]

        state = state_clazz(self, s_data)
        self.state = state

    def __init__(self, player, attrs={}, init=True):
        """
        Creates a Unit Container
        :param player:
        :param attrs:
        """
        self.player = player
        self.game = player.game
        self.Map = self.game.Map
        self.data = player.game.data
        self.Event = player.Event

        # Generate a unique id for this unit
        self.unit_id = self.generate_id()

        # Calculate dimension of this unit
        self.dimension = self._dimension()

        # Load initial state or accept existing state # TODO
        self.state = State.new(self, State.Despawned)
        self.state.transition(State.Spawning, attributes=attrs)

        if init:
            self.state.health = self.health_max

    def get_state(self):
        return self.current_state

    def process(self, tick):
        self.state.tick += tick
        self.state.update(tick)

    def unit_area(self):
        if not self.state.x or not self.state.y:
            return []
        map_tiles_x = [x for x in range(self.state.x, self.state.x + self.width)]
        map_tiles_y = [y for y in range(self.state.y, self.state.y + self.height)]
        tiles = list(itertools.product(map_tiles_x, map_tiles_y))
        return tiles

    def set_position(self, x, y):
        prev_x = self.state.x if self.state.x else x
        prev_y = self.state.y if self.state.y else y

        # If unit already have a position
        if self.state.x and self.state.y:
            self.clear_position_matrix()

        # Set new position
        self.state.x = x
        self.state.y = y

        # Update Matrix
        self.update_position_matrix()

        # Set direction of step
        self.set_direction(self.state.x - prev_x, self.state.y - prev_y)


    def set_direction(self, dx, dy):
        data = {
            (1, 0): UnitC.RIGHT,
            (-1, 0): UnitC.LEFT,
            (0, 1): UnitC.DOWN,
            (0, 0): UnitC.DOWN,
            (0, -1): UnitC.UP,
            (1, 1): UnitC.DOWN_RIGHT,
            (-1, 1): UnitC.DOWN_LEFT,
            (-1, -1): UnitC.UP_LEFT,
            (1, -1): UnitC.UP_RIGHT
        }
        try:
            self.state.direction = data[(dx, dy)]
        except:
            self.state.direction = data[(0, 0)]     # TODO!

    def clear_position_matrix(self):
        for x, y in ArrayUtil.get_area(
                self.state.x,
                self.state.y,
                self.width,
                self.height
        ):
            self.data['unit'][x][y] = 0
            self.data['unit_pid'][x][y] = 0

    def update_position_matrix(self):
        for x, y in ArrayUtil.get_area(
                self.state.x,
                self.state.y,
                self.width,
                self.height
        ):
            self.game.data['unit'][x][y] = self.unit_id
            self.game.data['unit_pid'][x][y] = self.player.id

    def add_to_game(self):
        self.game.units[self.unit_id] = self
        self.player.units.append(self.unit_id)

    def remove_from_game(self):
        del self.game.units[self.unit_id]
        self.player.units.remove(self.unit_id)

    def spawn(self):
        self.update_position_matrix()
        self.player.consumed_food += self.food_cost
        self.player.food += self.food
        self.state.spawned = True

    def despawn(self):
        self.clear_position_matrix()

        self.state.x = None
        self.state.y = None

        self.player.consumed_food -= self.food_cost
        self.player.food -= self.food

        self.state.spawned = False

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
        if self.structure == UnitC.STRUCTURE or self.speed <= 0:
            return False

        path = self.generate_path(x, y)

        self.state.transition(State.Walking, {
            'path': path,
            'path_goal': (x, y)
        })

    def distance(self, x, y, dim=0):
        d = math.hypot(self.state.x - (x + dim), self.state.y - (y + dim))
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

        mid_x = math.floor(width / 2)
        mid_y = math.floor(height / 2)

        return int((mid_x + mid_y) / 2)

    def center_tile(self):
        """
        Since self.state.x and self.state.y are the upper left corner,
        on structures which are 3x3 we need to determine center
        :return:
        """
        mid_x = math.floor(self.width / 2)
        mid_y = math.floor(self.height / 2)
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

            if self.state.has_next_state(State.Harvesting):
                self.state.clear_next()

            self.state.add_next(State.Harvesting, {
                'harvest_target': (x, y)
            }, 1)
            self.move(*tile)
        else:
            self.state.transition(State.Harvesting, {
                'harvest_target': (x, y)
            })

    def closest_recall_building(self):
        units = self.game.units
        recall_buildings = [units[x] for x in self.player.units if units[x].id in [
            UnitC.TOWN_HALL,
            UnitC.KEEP,
            UnitC.CASTLE
        ]]

        recall_buildings.sort(key=lambda x: self.distance(x.state.x, x.state.y), reverse=True)
        try:
            recall_building = recall_buildings.pop()
        except:
            return None

        return recall_building

    def is_walking(self):
        return self.state == self.states[UnitC.Walking]

    def is_worker(self):
        return self.can_harvest

    def get_damage(self, unit):
        # http://classic.battle.net/war2/basic/combat.shtml
        my_damage = (random.randint(self.damage_min, self.damage_max) - unit.armor) + self.damage_piercing
        return random.randint(math.floor(my_damage * .50), my_damage)

    def afflict_damage(self, damage):
        self.state.health = max(0, self.state.health - damage)

        if self.state.health <= 0:
            self.state.transition(State.Dead)

    def is_dead(self):
        return type(self.state) == Dead

    def attack(self, x, y):

        # Units with no minimum damage cannot attack
        # Warning! This makes unit not move to attack target, if they do not have damage!
        if self.damage_min <= 0:
            return

        attack_target = self.Map.get_unit(x, y)
        distance = self.distance(x, y)
        if distance > 1:

            tiles = self.Map.AdjacentMap.adjacent_walkable(
                attack_target.state.x + attack_target.dimension,
                attack_target.state.y + attack_target.dimension,
                attack_target.dimension + 1
            )

            if not tiles:
                return
            tile = self.shortest_distance(tiles)

            if self.state.has_next_state(State.Combat):
                self.state.clear_next()

            self.state.add_next(State.Combat, {
                'attack_target': attack_target.unit_id
            }, 2)
            self.move(*tile)
        else:
            self.state.transition(State.Combat, {
                'attack_target': attack_target.unit_id
            })

    def right_click(self, x, y):
        if not self.state.x or not self.state.y or self.state.type == State.Building.type:
            return

        if self.game.Map.is_attackable(self, x, y):
            self.attack(x, y)
        elif not self.structure and self.game.Map.is_harvestable_tile(self, x, y):
            self.harvest(x, y)
        elif self.game.Map.is_walkable_tile(self, x, y):
            self.state.clear_next()
            self.move(x, y)

    def can_build_here(self, subject):
        """
        This determines weither this unit can build a unit at current location
        :param subject:
        :param x:
        :param y:
        :return:
        """

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

            self.state.transition(State.Building, {
                'target_id': entity_class.id,
            })

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

