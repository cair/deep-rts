import itertools
import random
from math import *

from AI.Pathfinding import a_star_search
from Event import Event
from Mechanics.Constants import Unit as UnitC, Config
from Mechanics.Unit.States import Unspawned, Building, Combat, Idle, Dead, Harvesting
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

    # Walking
    path_timer = 0
    path_interval = .001 * Config.FRAME_MULTIPLIER
    path = []
    path_goal = None

    # Harvesting
    harvest_working = False
    harvest_interval = .5 * Config.FRAME_MULTIPLIER
    harvest_timer = 0
    harvest_iterator = 0
    harvest_tile = None

    # Creating (Building/Generating new unit)
    built_by = None
    building_entity = None  # Which building a unit is building
    build_timer = 0
    build_duration = 2
    build_complete = True

    # Attacking
    attack_target = None
    attack_timer = 0
    attack_interval = 1.25 * Config.FRAME_MULTIPLIER

    def __init__(self, player):
        self.unit_id = self.generate_id()
        self.x = None
        self.y = None
        self.direction = UnitC.DOWN


        self.states = {
            UnitC.Uncomplete: Unspawned(self),
            UnitC.Building: Building(self),
            UnitC.Combat: Combat(self),
            UnitC.Idle: Idle(self),
            UnitC.Dead: Dead(self),
            UnitC.Harvesting: Harvesting(self)

        }

        self.current_state = self.get_state_2(UnitC.Uncomplete)


        self.player = player
        self.game = player.game
        self.map = player.game.map
        self.unit_map = player.game.unit_map

        Event.notify(Event.Unit_Spawn, data=self.unit_id)

    def get_state_2(self, state):
        return self.states[state]

    def get_damage(self, unit):
        # http://classic.battle.net/war2/basic/combat.shtml
        my_damage = (random.randint(self.damage_min, self.damage_max) - unit.armor) + self.damage_piercing
        return random.randint(floor(my_damage * .50), my_damage)

    def attack(self, unit):
        my_damage = self.get_damage(unit)
        unit.health = max(0, unit.health - my_damage)
        Event.notify(Event.Attack, data=(self.unit_id, unit.unit_id))

        # if victim is idle (no path) it should retalliate)
        if unit.get_state() == UnitC.Idle:
            enemy_damage = unit.get_damage(self)
            self.health = max(0, self.health - enemy_damage)
            Event.notify(Event.Retaliate, data=(unit.unit_id, self.unit_id))

        if self.health == 0:
            self.die()
        if unit.health == 0:
            unit.die()

    def die(self):
        self.harvest_working = False
        self.building_entity = None
        self.attack_target = None
        self.path = []

        self.unit_map[self.x][self.y] = 0
        self.player.units.remove(self)
        del self.game.units[self.unit_id]

        self.x = None
        self.y = None

        Event.notify(Event.Unit_Died, data=self.unit_id)

    def get_state(self):

        # self.harvest_working
        # self.attack_target
        # self.built_by + self.built_by.build_complete
        # self.path
        # dead
        if not self.build_complete:
            return UnitC.Uncomplete
        if self.health == 0:
            return UnitC.Dead
        elif self.path:
            return UnitC.Walking
        elif self.harvest_working:
            return UnitC.Harvesting
        elif self.building_entity:
            return UnitC.Building
        elif self.attack_target:
            return UnitC.Combat
        else:
            return UnitC.Idle

    def nearby_harvestable_tile(self, x, y):
        tiles = ArrayUtil.neighbor_features_4(self.map.tiles, x, y)['harvestable']
        return tiles.pop()

    def start_harvesting(self, x, y, no_event=False):
        # Set harvest_work to true
        self.harvest_working = True
        self.harvest_iterator = UnitC.HARVEST_GOTO
        self.harvest_tile = self.nearby_harvestable_tile(x, y)

        if not no_event:
            Event.notify(Event.Unit_Start_Harvesting, data=self.unit_id)

    def end_harvesting(self):
        self.harvest_working = False
        self.harvest_iterator = UnitC.HARVEST_GOTO
        self.harvest_tile = None

        Event.notify(Event.Unit_Stop_Harvesting, data=self.unit_id)

    def distance(self, x, y, dim=0):
        center_tile = self.center_tile()
        d = hypot(center_tile[0] - x, center_tile[1] - y)
        return int(d) - dim

    def _crossover(self, x, y):
        dx1 = self.x - x
        dy1 = self.y - y

        cross = abs(dx1) + abs(dy1)
        return cross*0.001

    def shortest_distance(self, tiles):
        tiles.sort(key=lambda x: self.distance(x[0], x[1]) + self._crossover(x[0], x[1]), reverse=True)
        return tiles.pop()

    def center_tile(self):
        """
        Since self.x and self.y are the upper left corner,
        on structures which are 3x3 we need to determine center
        :return:
        """
        mid_x = floor(self.width / 2)
        mid_y = floor(self.height / 2)
        return self.x - mid_x, self.y - mid_y

    def dimension(self):
        mid_x = floor(self.width / 2)
        mid_y = floor(self.height / 2)

        return int((mid_x + mid_y) / 2)

    def closest_recall_building(self):
        recall_buildings = [x for x in self.player.units if x.id in [
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
            self.player.game.unit_map[self.x][self.y] = 0

        # Set new position
        self.x = x
        self.y = y

        Event.notify(Event.Unit_Set_Position, data=(self.unit_id, self.x, self.y))

        # Set direction of step
        self.set_direction(self.x - prev_x, self.y - prev_y)

        for tile in self.unit_area():
            self.player.game.unit_map[tile[0]][tile[1]] = self.unit_id

    def generate_path(self, x, y):
        path = a_star_search(
            self.player.game.map.tiles,
            self.player.game.unit_map,
            (self.x, self.y),
            (x, y),
            is_ground=self.ground_unit,
            is_water=self.water_unit
        )
        if path:
            path.pop()

        return path

    def move(self, x, y, is_harvesting=False, blacklist=[]):

        # Cannot move if not spawned on map
        # OR is a building
        if not self.x or not self.y or self.structure == UnitC.STRUCTURE:
            return False

        # Remove attack target (Cannot move and attack)
        self.attack_target = None

        target_tile = self.map.get_tile(x, y)
        target_tile_unit = self.game.get_unit(x, y)
        self.path_goal = (x, y)

        if self.ground_unit and target_tile['harvestable']:
            # Harvestable Tile
            # Return a list of tiles of target and alternative resources in near proximity
            tiles_resource = ArrayUtil.neighbor_features_4(self.map.tiles, x, y)
            walkables = tiles_resource['walkable']
            walkables = [x for x in walkables if self.game.unit_map[x[0]][x[1]] == UnitC.NONE or self.game.unit_map[x[0]][x[1]] == self.unit_id]
            harvestable = [x for x in tiles_resource['harvestable'] if x not in blacklist]

            if walkables:
                # Found a walkable tile at a resource til
                new_goal = self.shortest_distance(walkables)
                x = new_goal[0]
                y = new_goal[1]

                Event.notify(Event.Unit_Move_Harvestable, data=(self.unit_id, x, y))

                self.path = self.generate_path(x, y)

                if not self.harvest_tile:
                    self.start_harvesting(x, y)
                else:
                    self.start_harvesting(x, y, True)

            elif harvestable:
                # No walkable tile at (x, y) resource, select next available
                blacklist.append((x, y))
                new_resource = harvestable.pop()
                self.move(*new_resource, is_harvesting)
                return

        elif self.ground_unit and target_tile['ground'] and target_tile_unit == UnitC.NONE:
            # Ground, typically grass
            self.path = self.generate_path(x, y)

            if not is_harvesting and self.harvest_tile:
                self.end_harvesting()

            Event.notify(Event.Unit_Move, data=(self.unit_id, x, y))
            return

        elif target_tile_unit != UnitC.NONE:
            # Clicked a unit, go to and set attack state
            self.end_harvesting()
            self.attack_target = self.game.units[target_tile_unit]
            Event.notify(Event.Unit_Move_Attack, data=(self.unit_id, x, y))

    def process(self, tick):
        self.process_path(tick)
        self.process_harvest(tick)
        self.process_building(tick)
        self.process_attack(tick)

    def process_attack(self, tick):
        self.attack_timer += tick

        if self.attack_target:

            if self.attack_target.unit_id == self.unit_id \
                    or self.attack_target.player == self.player \
                    or self.attack_target.x is None:

                # Cannot attack self or friendly units or DEAD targets
                self.attack_target = None
                self.path = []
                return

            distance = self.distance(
                self.attack_target.x + self.attack_target.dimension(),
                self.attack_target.y + self.attack_target.dimension(),
                self.attack_target.dimension())

            if distance > 1 or (self.path and distance > 1):

                adjacent_tiles = \
                    self.map.walkable_neighbor_tiles(
                        self.attack_target.x + self.attack_target.dimension(),
                        self.attack_target.y + self.attack_target.dimension(),
                        self.attack_target.dimension())

                tile = self.shortest_distance(adjacent_tiles)
                self.path = self.generate_path(*tile)
            else:
                # Distance must be 0 or 1 aka can attack
                if self.attack_timer >= self.attack_interval:
                    self.attack(self.attack_target)
                    self.attack_timer = 0

    def process_path(self, tick):
        if self.path:

            self.path_timer += tick
            if self.path_timer > self.path_interval:

                next_step = self.path.pop()

                if self.unit_map[next_step[0]][next_step[1]] != UnitC.NONE:
                    self.move(*self.path_goal, self.harvest_working)
                    if not self.path:
                        return

                    next_step = self.path.pop()

                self.set_position(*next_step)
                self.path_timer = 0

    def process_harvest(self, tick):

        if self.harvest_working:

            if self.harvest_iterator == UnitC.HARVEST_GOTO:

                if self.distance(*self.harvest_tile) == 1:
                    # Nearby a harvestable tile
                    # go to next phase
                    self.harvest_iterator = UnitC.HARVEST_WORK
                elif not self.path:
                    self.move(*self.harvest_tile, True)

            elif self.harvest_iterator == UnitC.HARVEST_WORK:
                self.harvest_timer += tick

                if self.harvest_timer >= self.harvest_interval:
                    tile_data = self.map.get_tile(*self.harvest_tile)

                    if self.inventory_lumber + self.inventory_gold + self.inventory_oil >= self.inventory_space:
                        # Inventory is full, return to base
                        self.harvest_iterator = UnitC.HARVEST_RECALL
                        return

                    self.inventory_gold += tile_data['gold_yield']
                    self.inventory_lumber += tile_data['lumber_yield']
                    self.inventory_oil += tile_data['oil_yield']

                    self.harvest_timer = 0

            elif self.harvest_iterator == UnitC.HARVEST_RECALL:

                if not self.path:
                    # No path back to base is created. Find base and set path
                    recall_target = self.closest_recall_building()

                    if not recall_target:
                        self.harvest_working = False
                        self.harvest_iterator = UnitC.HARVEST_GOTO
                        return

                    # Find distance to recall building
                    distance = self.distance(
                        recall_target.x + recall_target.dimension(),
                        recall_target.y + recall_target.dimension(),
                        recall_target.dimension()
                    )

                    if distance == 1:
                        # Already at a point, no pathfinding required
                        self.harvest_iterator = UnitC.HARVEST_DELIVER
                        return

                    # Find all neighbor tiles around recall building
                    available_tiles = self.map.walkable_neighbor_tiles(
                        recall_target.x + recall_target.dimension(),
                        recall_target.y + recall_target.dimension(),
                        recall_target.dimension()
                    )

                    if not available_tiles:
                        return

                    # Find path to the shortest one
                    recall_tile = self.shortest_distance(available_tiles)

                    self.move(*recall_tile, True)
                    self.harvest_iterator = UnitC.HARVEST_DELIVER

            elif self.harvest_iterator == UnitC.HARVEST_DELIVER:

                recall_target = self.closest_recall_building()
                distance = self.distance(
                    recall_target.x + recall_target.dimension(),
                    recall_target.y + recall_target.dimension(),
                    recall_target.dimension()
                )

                if distance == 1:
                    # Can deliver
                    # Deliver resources
                    self.player.lumber += self.inventory_lumber
                    self.player.gold += self.inventory_gold
                    self.player.oil += self.inventory_oil

                    # Reset inventory
                    self.inventory_gold, self.inventory_oil, self.inventory_lumber = 0, 0, 0

                    # Set iterator back to start (GOTO)
                    self.harvest_iterator = UnitC.HARVEST_GOTO
                elif distance > 1 and not self.path:
                    # Failed to deliver (collision most likely)
                    self.harvest_iterator = UnitC.HARVEST_RECALL

    def process_building(self, tick):

        if not self.build_complete:
            self.build_timer += tick
            if self.build_timer >= self.build_duration:
                self.build_complete = True
                self.built_by.building_entity = None  # Remove reference because builder is DONE
                self.direction = UnitC.UP
                self.build_timer = 0

                if self.structure == UnitC.UNIT:
                    available_spawn_tiles = self.map.walkable_neighbor_tiles(
                        self.built_by.x + self.built_by.dimension(),
                        self.built_by.y + self.built_by.dimension(),
                        self.built_by.dimension()
                    )
                    if available_spawn_tiles:
                        spawn_tile = random.choice(available_spawn_tiles)
                        self.x, self.y = spawn_tile
                        self.spawn()
                    else:
                        self.build_complete = False

                if self.built_by.x is None or self.built_by.y is None:
                    available_spawn_tiles = self.map.walkable_neighbor_tiles(
                        self.x + self.dimension(),
                        self.y + self.dimension(),
                        self.dimension()
                    )
                    if available_spawn_tiles:
                        spawn_tile = random.choice(available_spawn_tiles)
                        self.built_by.x, self.built_by.y = spawn_tile
                        self.built_by.spawn()
                    else:
                        self.build_complete = False

    def build(self, idx):
        """
        Build a unit or structure
        :param idx: Index of (From units inventory)
        :return: Did build or not
        """

        if not self.build_complete:
            print("Must be completed to build units!")
            return False

        # Created a entity (Either a building or a unit (ie: worker))
        entity = self.can_build[idx](self.player)
        entity.x, entity.y = self.x, self.y
        entity.built_by = self
        entity.build_timer = 0
        entity.build_complete = False

        if self.player.consumed_food + entity.food_cost > self.player.food:
            print("Not enough food!")
            return False

        if self.player.gold < entity.cost_gold or self.player.lumber < entity.cost_lumber:
            print("Cannot afford this unit")
            return False

        can_build = self.map.can_build_here(
            self,
            entity.x,
            entity.y,
            entity.dimension()
        )

        entity.x, entity.y = entity.center_tile()

        if can_build:
            Event.notify(Event.Unit_Build, data=(self.unit_id, entity.unit_id, entity.id))

            self.player.gold -= entity.cost_gold
            self.player.lumber -= entity.cost_lumber
            self.building_entity = entity

            if entity.structure == UnitC.STRUCTURE and self.structure == UnitC.UNIT:
                # A unit is building a structure (Worker building Keep)
                self.despawn()                  # Despawn Worker from game
                entity.spawn()                  # Spawn Entity into game

            elif entity.structure == UnitC.UNIT and self.structure == UnitC.STRUCTURE:

                # Spawn Entity into game
                self.game.units[entity.unit_id] = entity # TODO ??
                self.player.units.append(entity)

            return True

    def spawn(self):
        for x, y in ArrayUtil.get_area(self.x, self.y, self.width, self.height):
            self.unit_map[x][y] = self.unit_id
        self.game.units[self.unit_id] = self

        if self not in self.player.units:
            self.player.units.append(self)

        self.player.consumed_food += self.food_cost
        self.player.food += self.food

    def despawn(self):
        self.unit_map[self.x][self.y] = 0
        del self.game.units[self.unit_id]
        self.path = []
        self.attack_target = None
        self.x = None
        self.y = None

        self.player.consumed_food -= self.food_cost
        self.player.food -= self.food

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
                self.direction = data[(0, 0)]

    def right_click(self, x, y):

        # Do not allow out of bounds clicks
        try:
            self.unit_map[x][y]
        except:
            return

        # Move
        self.move(x, y)



    @staticmethod
    def generate_id():
        unit_id = Unit.UNIT_COUNTER
        Unit.UNIT_COUNTER += 1
        return unit_id

