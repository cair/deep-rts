import itertools
import random
import math
from collections import deque

from game import Config
from game import Constants
from game.unit.UnitManager import UnitManager
from game.util.Pathfinding import a_star_search


class Unit:

    def __init__(self, player):
        self.player = player
        self.id = len(player.game.units)
        self.state_manager = player.game.state_manager
        self.state = player.game.state_manager.despawned_state

        self.animation_interval = .5 * Config.TICK_MULTIPLIER
        self.animation_timer = 0
        self.animation_iterator = 0

        self.type_id = Constants.Unit.Nothing
        self.health = None
        self.health_max = None
        self.direction = None

        self.ground_unit = None
        self.water_unit = None

        self.damage_min = -1
        self.damage_max = -1
        self.damage_range = -1
        self.damage_piercing = -1
        self.armor = -1

        self.lumber_carry = 0
        self.gold_carry = 0
        self.oil_carry = 0
        self.carry_capacity = 10
        self.build_inventory = None
        self.speed = None
        self.sight = None
        self.range = 4

        self.can_harvest = None
        self.can_attack = None
        self.can_move = None
        self.military = None
        self.structure = None
        self.recallable = False

        self.lumber_cost = None
        self.gold_cost = None
        self.oil_cost = None

        self.food_production = None
        self.food_consumption = None

        self.width = None
        self.height = None

        self.name = None
        self.state = self.state_manager.despawned_state
        self.state_queue = deque()

        # Spawning attributes
        self.spawn_duration = None
        self.spawn_timer = None
        self.spawn_tile_id = Constants.Tile.Nothing
        self.built_by_id = Constants.Unit.Nothing

        # Harvesting attributes
        self.harvest_interval = .5 * Config.TICK_MULTIPLIER
        self.harvest_timer = None
        self.harvest_iterator = None
        self.harvest_target_id = Constants.Unit.Nothing

        # Building attributes
        self.combat_target_id = Constants.Unit.Nothing
        self.combat_timer = 1000
        self.combat_interval = 1 * Config.TICK_MULTIPLIER

        # Walking
        self.walking_timer = 0
        self.walking_path = []
        self.walking_goal_id = Constants.Tile.Nothing
        self.walking_interval = .1 * Config.TICK_MULTIPLIER



    def update(self):
        self.state.update(self)

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

    def spawn(self, spawn_tile, build_timer_value):
        self.spawn_timer = build_timer_value
        self.spawn_tile_id = spawn_tile.id
        self.transition_state_next(self.state_manager.spawn_state)
        self.enqueue_state(self.state_manager.idle_state)

    def transition_state_next(self, next_state):
        self.state.end(self)
        self.state = next_state
        self.state.init(self)

    def transition_state(self):
        if not self.state_queue:
            # Has state
            next_state = self.state_manager.idle_state
            self.transition_state_next(next_state)
            return

        next_state = self.state_queue.pop()
        self.state.end(self)
        self.state = next_state
        return



    def enqueue_state(self, state):
        self.state_queue.append(state)

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

    def build(self, idx):

        if self.state.id != Constants.State.Idle:
            return False

        if idx < 0 or idx >= len(self.build_inventory):
            return False

        build_unit_idx = self.build_inventory[idx]
        new_unit = UnitManager.construct_unit(build_unit_idx, self.player)


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
