import itertools
import random
import math

from game import state
from game import Config
from game import const
from game.util.Pathfinding import a_star_search
from game.util.PriorityQueue import PriorityQueue


class Unit:
    UNIT_COUNTER = 1

    id = const.Unit.ID_NONE

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
    military = False

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
            'unit_id': self.unit_id,
            'state': dict(self.d)
        }

    def load(self, data, uid):
        s_data = data['state']
        self.d = {
            'x': s_data['x'],
            'y': s_data['y'],
            'tick': s_data['tick'],
            'inventory_lumber': s_data['inventory_lumber'],
            'inventory_gold': s_data['inventory_gold'],
            'inventory_oil': s_data['inventory_oil'],
            'spawned': s_data['spawned'],
            'health': s_data['health'],
            'direction': s_data['direction'],
            'current_state': s_data['current_state'],
            's': {
                state.Building.type: {
                    'build_timer': s_data['s'][state.Building.type]['build_timer'],
                    'build_duration': s_data['s'][state.Building.type]['build_duration'],
                    'target_id': s_data['s'][state.Building.type]['target_id'],
                    'entity_id': s_data['s'][state.Building.type]['entity_id'],
                },
                state.Combat.type: {
                    'attack_target': s_data['s'][state.Combat.type]['attack_target'],
                    'attack_timer': s_data['s'][state.Combat.type]['attack_timer'],
                },
                state.Idle.type: {},
                state.Dead.type: {},
                state.Harvesting.type: {
                    'harvest_target': s_data['s'][state.Harvesting.type]['harvest_target'],
                    'harvest_timer': s_data['s'][state.Harvesting.type]['harvest_timer'],
                    'harvest_iterator': s_data['s'][state.Harvesting.type]['harvest_iterator']
                },
                state.Spawning.type: {},
                state.Walking.type: {
                    'path_timer': s_data['s'][state.Walking.type]['path_timer'],
                    'path': s_data['s'][state.Walking.type]['path'],
                    'path_goal': s_data['s'][state.Walking.type]['path_goal'],
                },
                state.Despawned.type: {}
            }
        }
        self.init()
        #print(self.state, uid, type(self), self.state.data)

    def __init__(self, player, init=True):
        """
        Creates a Unit Container
        :param player:
        :param attrs:
        """
        self.player = player
        self.game = player.game
        self.Map = self.game.Map
        self.AdjacentMap = self.game.AdjacentMap
        self.data = player.game.data
        self.Event = player.Event


        self.buildable = []

        # Calculate dimension of this unit
        self.dimension = self._dimension()

        self.states = {}
        self.state = None
        if init:
            # Generate a unique id for this unit
            self.unit_id = str(len(self.game.units) + 1)
            self.d = {
                'x': None,
                'y': None,
                'tick': 0,
                'inventory_lumber': 0,
                'inventory_gold': 0,
                'inventory_oil': 0,
                'spawned': False,
                'health': self.health_max,
                'direction': const.Unit.DIRECTION_DOWN,
                'current_state': const.State.ID_Spawning,
                's': {
                    state.Building.type: {
                        'build_timer': None,
                        'build_duration': None,
                        'target_id': None,
                        'entity_id': None,
                    },
                    state.Combat.type: {
                        'attack_target': None,
                        'attack_timer': 1000,
                    },
                    state.Idle.type: {},
                    state.Dead.type: {},
                    state.Harvesting.type: {
                        'harvest_target': None,
                        'harvest_timer': 0,
                        'harvest_iterator': 1
                    },
                    state.Spawning.type: {},
                    state.Walking.type: {
                        'path_timer': None,
                        'path': None,
                        'path_goal': None,
                    },
                    state.Despawned.type: {}
                }
            }
            self.init()


    def init(self):
        # Dictionary over all possible states for this unit, Reuse objects
        self.states = {'q': PriorityQueue([])}
        self.states[state.Building.type] = state.Building(self)
        self.states[state.Combat.type] = state.Combat(self)
        self.states[state.Idle.type] = state.Idle(self)
        self.states[state.Dead.type] = state.Dead(self)
        self.states[state.Harvesting.type] = state.Harvesting(self)
        self.states[state.Spawning.type] = state.Spawning(self)
        self.states[state.Walking.type] = state.Walking(self)
        self.states[state.Despawned.type] = state.Despawned(self)
        self.state = self.states[self.d['current_state']]

    def get_state(self):
        return self.state

    def process(self, tick):
        self.d['tick'] += tick
        self.state.update(tick)

    def unit_area(self):
        if not self.d['x'] or not self.d['y']:
            return []
        map_tiles_x = [x for x in range(self.d['x'], self.d['x'] + self.width)]
        map_tiles_y = [y for y in range(self.d['y'], self.d['y'] + self.height)]
        tiles = list(itertools.product(map_tiles_x, map_tiles_y))
        return tiles

    def set_position(self, x, y):
        prev_x = self.d['x'] if self.d['x'] else x
        prev_y = self.d['y'] if self.d['y'] else y

        # If unit already have a position
        if self.d['x'] and self.d['y']:
            self.clear_position_matrix()

        # Set new position
        self.d['x'] = x
        self.d['y'] = y

        # Update Matrix
        self.update_position_matrix()

        # Set direction of step
        self.set_direction(self.d['x'] - prev_x, self.d['y'] - prev_y)


    def set_direction(self, dx, dy):
        data = {
            (1, 0): const.Unit.DIRECTION_RIGHT,
            (-1, 0): const.Unit.DIRECTION_LEFT,
            (0, 1): const.Unit.DIRECTION_DOWN,
            (0, 0): const.Unit.DIRECTION_DOWN,
            (0, -1): const.Unit.DIRECTION_UP,
            (1, 1): const.Unit.DIRECTION_DOWN_RIGHT,
            (-1, 1): const.Unit.DIRECTION_DOWN_LEFT,
            (-1, -1): const.Unit.DIRECTION_UP_LEFT,
            (1, -1): const.Unit.DIRECTION_UP_RIGHT
        }
        try:
            self.d['direction'] = data[(dx, dy)]
        except:
            self.d['direction'] = data[(0, 0)]     # TODO!

    def clear_position_matrix(self):
        for x, y in self.unit_area():
            del self.data['unit'][x, y]
            del self.data['unit_pid'][x, y]

    def update_position_matrix(self):
        for x, y in self.unit_area():
            self.game.data['unit'][x, y] = self.unit_id
            self.game.data['unit_pid'][x, y] = self.player.id

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
        self.d['spawned'] = True

    def despawn(self):
        self.clear_position_matrix()

        self.d['x'] = None
        self.d['y'] = None

        self.player.consumed_food -= self.food_cost
        self.player.food -= self.food

        self.d['spawned'] = False

    def generate_path(self, x, y):
        path = a_star_search(self.game, self.d['x'], self.d['y'], x, y)

        if path:
            path.pop()

        return path

    def move(self, x, y):
        # Cannot move if building
        if self.structure == const.Unit.TYPE_STRUCTURE or self.speed <= 0:
            return False

        path = [(x, y)] if self.distance(x, y ) == 1 else self.generate_path(x, y)

        self.state.transition(const.State.ID_Walking, {
            'path': path,
            'path_goal': (x, y)
        })

    def void(self):
        """
        A "do nothing" action
        :return:
        """
        return True

    def distance(self, x, y, dim=0):
        d = math.hypot(self.d['x'] - (x + dim), self.d['y'] - (y + dim))
        return int(d) - dim

    def _crossover(self, x, y):
        dx1 = self.d['x'] - x
        dy1 = self.d['y'] - y

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
        Since self.d['x'] and self.d['y'] are the upper left corner,
        on structures which are 3x3 we need to determine center
        :return:
        """
        mid_x = math.floor(self.width / 2)
        mid_y = math.floor(self.height / 2)
        return self.d['x'] - mid_x, self.d['y'] - mid_y

    def harvest(self, x, y):
        if not self.can_harvest:
            return

        distance = self.distance(x, y)
        if distance > 1:
            tiles = self.game.AdjacentMap.adjacent_walkable(self.game, x, y, 1)

            if not tiles:
                return
            tile = self.shortest_distance(tiles)

            if self.state.has_next_state(const.State.ID_Harvesting):
                self.state.clear_next()

            self.state.add_next(const.State.ID_Harvesting, {
                'harvest_target': (x, y)
            }, 1)
            self.move(*tile)
        else:
            self.state.transition(const.State.ID_Harvesting, {
                'harvest_target': (x, y)
            })

    def closest_recall_building(self):
        units = self.game.units
        recall_buildings = [units[x] for x in self.player.units if units[x].id in [
            const.Unit.ID_TOWN_HALL,
            const.Unit.ID_KEEP,
            const.Unit.ID_CASTLE
        ]]

        recall_buildings.sort(key=lambda x: self.distance(x.d['x'], x.d['y']), reverse=True)
        try:
            recall_building = recall_buildings.pop()
        except:
            return None

        return recall_building

    def is_walking(self):
        return self.state.type == const.State.ID_Walking

    def is_worker(self):
        return self.can_harvest

    def get_damage(self, unit):
        # http://classic.battle.net/war2/basic/combat.shtml
        my_damage = (random.randint(self.damage_min, self.damage_max) - unit.armor) + self.damage_piercing
        return random.randint(math.floor(my_damage * .50), my_damage)

    def afflict_damage(self, damage):
        self.d['health'] = max(0, self.d['health'] - damage)

        if self.d['health'] <= 0:
            self.state.transition(const.State.ID_Dead)

            # In addition check for @see issue #1
            self.player.calculate_defeat()
            self.game.calculate_winner()

    def is_dead(self):
        return self.state.type == const.State.ID_Dead

    def attack(self, x, y):

        # Units with no minimum damage cannot attack
        # Warning! This makes unit not move to attack target, if they do not have damage!
        if self.damage_min <= 0:
            return

        attack_target = self.game.get_unit(x, y)
        distance = self.distance(x, y)
        if distance > 1:

            tiles = self.AdjacentMap.adjacent_walkable(
                self.game,
                attack_target.d['x'] + attack_target.dimension,
                attack_target.d['y'] + attack_target.dimension,
                attack_target.dimension + 1
            )

            if not tiles:
                return
            tile = self.shortest_distance(tiles)

            self.state.add_next(const.State.ID_Combat, {
                'attack_target': attack_target.unit_id
            }, 2)

            self.move(*tile)
        else:
            self.state.transition(const.State.ID_Combat, {
                'attack_target': attack_target.unit_id
            })

    def right_click(self, x, y):
        if not self.d['x'] or not self.d['y'] or self.state.type == const.State.ID_Building:
            return
        if self.game.Map.is_attackable(self, x, y):
            self.states['q'].clear()
            self.attack(x, y)
        elif self.structure == const.Unit.TYPE_UNIT and self.game.Map.is_harvestable_tile(self, x, y):
            self.states['q'].clear()
            self.harvest(x, y)
        elif self.game.Map.is_walkable_tile(self, x, y):
            self.states['q'].clear()
            self.move(x, y)

    def can_build(self, entity):

        if self.player.consumed_food + entity.food_cost > self.player.food:
            #print("Not enough food!")
            return False

        if self.player.gold < entity.cost_gold or self.player.lumber < entity.cost_lumber:
            #print("Cannot afford this unit")
            return False

        if self.state.type != const.State.ID_Idle and self.state.type != const.State.ID_Walking:
            #print("Wrong state")
            return False

        return True

    def can_build_here(self, subject):
        """
        This determines weither this unit can build a unit at current location
        :param subject:
        :param x:
        :param y:
        :return:
        """

        if self.structure == const.Unit.TYPE_STRUCTURE:
            adjacent_tiles = self.AdjacentMap.adjacent_walkable(
                self.game,
                self.d['x'] + self.dimension,
                self.d['y'] + self.dimension,
                self.dimension + 1
            )
            return len(adjacent_tiles) > 0
        else:

            # A unit builds a structure
            adjacent_tiles = self.AdjacentMap.adjacent_walkable(self.game, self.d['x'], self.d['y'], Unit._dimension(subject))
            adjacent_tiles.append((self.d['x'], self.d['y']))
            subject_area = list(
                itertools.product(
                    list(range(self.d['x'] - Unit._dimension(subject), self.d['x'] - Unit._dimension(subject) + subject.height)),
                    list(range(self.d['y'] - Unit._dimension(subject), self.d['y'] - Unit._dimension(subject) + subject.width))
                ))
            common = list(set(adjacent_tiles).intersection(set(subject_area)))
            return len(common) == len(subject_area)

    def build(self, idx, force_allow=False):
        """
        # Triggers a build event given index in the inventory of the unit. A flag may also be set which overrides can_build constraint
        :param idx:
        :param force_allow:
        :return:
        """
        entity_class = self.buildable[idx]
        if not self.can_build(entity_class) and not force_allow:
            return False

        if self.can_build_here(entity_class):
            # All tests passed, unit can be built
            # Subtract resources
            self.player.gold -= entity_class.cost_gold
            self.player.lumber -= entity_class.cost_lumber

            self.player.statistics['unit_count'] += 1

            self.state.transition(const.State.ID_Building, {
                'target_id': entity_class.id,
                'build_duration': entity_class.build_duration,
                'build_timer': 0
            })

            return True
        else:
            # TODO feedback could not build
            return False
            pass
