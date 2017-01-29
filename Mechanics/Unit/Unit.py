import itertools
import random
from math import *

from AI.Pathfinding import a_star_search
from Event import Event
from Mechanics.Constants import Unit as UnitC, Config, Map as MapC
from Mechanics.Map import Map
from Mechanics.Unit.States import Despawned, Building, Combat, Idle, Dead, Harvesting, Spawning, Walking, State
from Mechanics.Util import ArrayUtil


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
    health = None
    health_max = None
    armor = None
    cost_gold = None
    cost_lumber = None
    inventory_lumber = 0
    inventory_gold = 0
    inventory_oil = 0
    inventory_space = 10
    speed = None
    sight = None
    food = 0
    food_cost = 1
    range = 1

    can_harvest = False



    harvest_target = None

    def __init__(self, player, attrs={}):
        self.unit_id = self.generate_id()
        self.x = None
        self.y = None
        self.tick = 0
        self.direction = UnitC.DOWN
        self.dimension = self._dimension()

        self.player = player
        self.game = player.game
        self.data = player.game.data

        self.state = State.new(self, State.Despawned)
        self.state.transition(State.new(self, State.Spawning, properties=attrs))

        Event.notify(Event.Unit_Spawn, data=self.unit_id)

    def get_state(self):
        return self.current_state

    def process(self, tick):
        self.tick += tick
        self.state.update(tick)

    def unit_area(self):
        map_tiles_x = [x for x in range(self.x, self.x + self.width)]
        map_tiles_y = [y for y in range(self.y, self.y + self.height)]
        tiles = list(itertools.product(map_tiles_x, map_tiles_y))
        return tiles

    def set_position(self, x, y):
        prev_x = self.x if self.x else x
        prev_y = self.y if self.y else y

        # If unit already have a position
        if self.x and self.y:
            self.clear_position_matrix()

        # Set new position
        self.x = x
        self.y = y

        # Update Matrix
        self.update_position_matrix()

        # Set direction of step
        self.set_direction(self.x - prev_x, self.y - prev_y)

        Event.notify(Event.Unit_Set_Position, data=(self.unit_id, self.x, self.y))

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
            self.direction = data[(dx, dy)]
        except:
            self.direction = data[(0, 0)] # TODO!

    def clear_position_matrix(self):
        for x, y in ArrayUtil.get_area(
                self.x,
                self.y,
                self.width,
                self.height
        ):
            self.data['unit'][x][y] = 0
            self.data['unit_pid'][x][y] = 0

    def update_position_matrix(self):
        for x, y in ArrayUtil.get_area(
                self.x,
                self.y,
                self.width,
                self.height
        ):
            self.game.data['unit'][x][y] = self.unit_id
            self.game.data['unit_pid'][x][y] = self.player.id

    def spawn(self):
        self.game.units[self.unit_id] = self
        self.player.units.append(self.unit_id)
        self.update_position_matrix()
        self.player.consumed_food += self.food_cost
        self.player.food += self.food

    def despawn(self):

        del self.game.units[self.unit_id]
        self.player.units.remove(self.unit_id)
        self.clear_position_matrix()

        self.x = None
        self.y = None

        self.player.consumed_food -= self.food_cost
        self.player.food -= self.food

    def generate_path(self, x, y):
        path = a_star_search(
            self.game.data['tile'],
            self.game.data['unit'],
            (self.x, self.y),
            (x, y),
            is_ground=self.ground_unit,
            is_water=self.water_unit
        )

        if path:
            path.pop()

        return path

    def move(self, x, y):
        # Cannot move if building
        if self.structure == UnitC.STRUCTURE or self.speed <= 0:
            return False

        path = self.generate_path(x, y)

        self.state.transition(State.new(self, State.Walking, {
            'path': path,
            'path_goal': (x, y)
        }))

        Event.notify(Event.Unit_Move, data=(self.unit_id, x, y))

    def distance(self, x, y, dim=0):
        d = hypot(self.x - (x + dim), self.y - (y + dim))
        return int(d) - dim

    def _crossover(self, x, y):
        dx1 = self.x - x
        dy1 = self.y - y

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
        Since self.x and self.y are the upper left corner,
        on structures which are 3x3 we need to determine center
        :return:
        """
        mid_x = floor(self.width / 2)
        mid_y = floor(self.height / 2)
        return self.x - mid_x, self.y - mid_y

    def harvest(self, x, y):
        if not self.can_harvest:
            return

        distance = self.distance(x, y)
        if distance > 1:
            tiles = ArrayUtil.adjacent_walkable_tiles(self, x, y, 1)
            if not tiles:
                return
            tile = self.shortest_distance(tiles)

            if not self.state.has_next_state(State.Harvesting):
                self.state.add_next(State.new(self, State.Harvesting, {
                    'harvest_target': (x, y)
                }), 1)
            self.move(*tile)
        else:
            self.state.transition(State.new(self, State.Harvesting, {
                'harvest_target': (x, y)
            }))

    def closest_recall_building(self):
        units = self.game.units
        recall_buildings = [units[x] for x in self.player.units if units[x].id in [
            UnitC.TOWN_HALL,
            UnitC.KEEP,
            UnitC.CASTLE
        ]]

        recall_buildings.sort(key=lambda x: self.distance(x.x, x.y), reverse=True)
        try:
            recall_building = recall_buildings.pop()
        except:
            return None

        return recall_building

    def is_walking(self):
        return self.state == self.states[UnitC.Walking]

    def get_damage(self, unit):
        # http://classic.battle.net/war2/basic/combat.shtml
        my_damage = (random.randint(self.damage_min, self.damage_max) - unit.armor) + self.damage_piercing
        return random.randint(floor(my_damage * .50), my_damage)

    def afflict_damage(self, damage):
        self.health = max(0, self.health - damage)

        if self.health <= 0:
            self.state.transition(State.new(self, State.Dead))


    def is_dead(self):
        return self.state.type == self.state.Dead

    def attack(self, x, y):

        # Units with no minimum damage cannot attack
        # Warning! This makes unit not move to attack target, if they do not have damage!
        if self.damage_min <= 0:
            return

        attack_target = Map.get_unit(self, x, y)
        distance = self.distance(x, y)
        if distance > 1:
            tiles = ArrayUtil.adjacent_walkable_tiles(self, x, y, 1)
            if not tiles:
                return
            tile = self.shortest_distance(tiles)

            if not self.state.has_next_state(State.Harvesting):
                self.state.add_next(State.new(self, State.Combat, {
                    'attack_target': attack_target
                }), 1)
            self.move(*tile)
        else:
            self.state.transition(State.new(self, State.Combat, {
                'attack_target': attack_target
            }))

    def right_click(self, x, y):
        if self.game.Map.is_attackable(self, x, y):
            self.attack(x, y)
        elif not self.structure and self.game.Map.is_harvestable_tile(self, x, y):
            self.harvest(x, y)
        elif self.game.Map.is_walkable_tile(self, x, y):
            self.move(*(x, y))

    def build(self, idx):

        entity_class = self.buildable[idx]

        if self.player.consumed_food + entity_class.food_cost > self.player.food:
            print("Not enough food!")
            return False

        if self.player.gold < entity_class.cost_gold or self.player.lumber < entity_class.cost_lumber:
            print("Cannot afford this unit")
            return False

        can_build = Map.buildable_here(
            self,
            self.x,
            self.y,
            self._dimension(entity_class)
        )

        if can_build:
            # All tests passed, unit can be built

            # Created a entity (Either a building or a unit (ie: worker))
            entity = entity_class(self.player, {
                'spawned_by': self
            })
            entity.x, entity.y = self.x - entity.dimension, self.y - entity.dimension

            # Subtract resources
            self.player.gold -= entity_class.cost_gold
            self.player.lumber -= entity_class.cost_lumber

            if self.structure and not entity.structure:
                # Building constructs Unit
                self.state.transition(State.new(self, State.Building, {
                    'target': entity,
                }))

            elif not self.structure and entity.structure:
                # Unit constructs Building
                self.state.transition(State.new(self, State.Building, {
                    'target': entity
                }))
                self.despawn()
                entity.spawn()

    @staticmethod
    def generate_id():
        unit_id = Unit.UNIT_COUNTER
        Unit.UNIT_COUNTER += 1
        return unit_id

