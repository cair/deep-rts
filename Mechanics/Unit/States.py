from Mechanics.Constants import Config
from Mechanics.Constants import Unit as UnitC
from Mechanics.Map import Map
from Mechanics.Util import ArrayUtil
from PriorityQueue import PriorityQueue


class State:
    Idle = 0
    Building = 1
    Harvesting = 2
    Combat = 3
    Walking = 4
    Dead = 5
    Spawning = 6
    Despawned = 7
    mapping = {}

    type = None

    @staticmethod
    def new(unit, state_constant, properties={}):
        state = State.mapping[state_constant](unit)
        for key in properties:
            setattr(state, key, properties[key])
        state.init()
        return state

    def init(self):
        pass

    @staticmethod
    def type_of(state_constant):
        return State.mapping[state_constant]

    def clear_next(self):
        self.next_states = PriorityQueue(self.unit)

    def has_next_state(self, state_constant):
        for s in self.next_states:
            if self.type_of(state_constant) == type(s):
                return True
        return False

    def __repr__(self):
        return '<%s@%s>' % (
            self.__class__.__name__,
            hex(id(self))
        )

    def __init__(self, unit):
        self.unit = unit
        self.player = self.unit.player
        self.Event = self.unit.Event
        self.game = unit.game
        self.Map = self.game.Map
        self.next_states = PriorityQueue(unit)

    def transition(self, force_state=None):
        if force_state:
            next_state = force_state
        else:
            next_state = self.next_states.get()

        if next_state:
            next_state.next_states = self.next_states
            #print("%s. Transition from %s | %s => %s (%s => %s)" % (self.unit.tick, self.unit, self.id, next_state.id, self, next_state))
            self.unit.state = next_state
        else:
            # No next state, Idle
            self.transition(State.new(self.unit, State.Idle))

    def update(self, dt):
        pass

    def add_next(self, next_state, pri):
        self.next_states.put(next_state, pri)


class Spawning(State):
    id = "Spawning"
    type = State.Spawning

    # Creating (Building/Generating new unit)
    build_timer = 20
    build_duration = 20
    build_complete = False
    spawned_by = None

    def __init__(self, unit):
        super().__init__(unit)

    def update(self, tick):
        pass


class Despawned(State):
    id = "Despawned"
    type = State.Despawned

    def __init__(self, unit):
        super().__init__(unit)

        self.done = False

    def update(self, dt):
        if not self.done:
            self.unit.despawn()
            # self.transition()


class Walking(State):
    id = "Walking"
    type = State.Walking

    # Walking
    path_timer = 0
    path_interval = .001 * Config.FRAME_MULTIPLIER
    path = []
    path_goal = None

    def __init__(self, unit):
        super().__init__(unit)

    def update(self, tick):
        if self.path:

            self.path_timer += tick
            if self.path_timer > self.path_interval:

                next_step = self.path.pop()

                if not Map.is_walkable_tile(self.unit, *next_step):
                    self.transition()
                    return

                self.unit.set_position(*next_step)
                self.path_timer = 0
        else:
            # Nothing to do. Transition to next state
            self.transition()


class Combat(State):
    id = "Combat"
    type = State.Combat

    # Attacking
    attack_target = None
    attack_timer = 1000
    attack_interval = 1.25 * Config.FRAME_MULTIPLIER

    def __init__(self, unit):
        super().__init__(unit)

    def update(self, tick):
        self.attack_timer += tick

        if self.attack_timer >= self.attack_interval:
            distance = self.unit.distance(self.attack_target.x, self.attack_target.y, self.attack_target.dimension)
            if distance > self.unit.range:
                # Too far away, Walk

                # Find adjacent tile to the attack target
                tiles = self.Map.AdjacentMap.adjacent_walkable(
                    self.attack_target.x + self.attack_target.dimension,
                    self.attack_target.y + self.attack_target.dimension,
                    self.attack_target.dimension + 1
                )
                # No tiles are available around the target
                if not tiles:
                    return

                # Find the tile with shortest distance
                tile = self.unit.shortest_distance(tiles)

                # Next state is Combat (After movement)
                self.add_next(State.new(self.unit, State.Combat, {
                    'attack_target': self.attack_target
                }), 1)

                # Move to tile (This triggers transition)
                self.unit.move(*tile)
            else:
                # Can attack (Distance < Range)
                my_damage = self.unit.get_damage(self.unit)
                self.attack_target.afflict_damage(my_damage)
                self.Event.notify(self.Event.Attack, data=(self.unit.unit_id, self.attack_target.unit_id))

                # If attack target is dead, transition to next state
                if self.attack_target.is_dead():
                    self.player.statistics['kill_count'] += 1
                    self.transition()
                    return

                # Make unit retaliate if its Idle (Changes to attack state)
                if type(self.attack_target.state) == Idle:
                    self.attack_target.attack(self.unit.x, self.unit.y)

            self.attack_timer = 0


class Idle(State):
    id = "Idle"
    type = State.Idle

    def __init__(self, unit):
        super().__init__(unit)


class Dead(State):
    id = "Dead"
    type = State.Dead

    def __init__(self, unit):
        super().__init__(unit)

    def init(self):
        self.unit.despawn()
        self.unit.remove_from_game()

    def update(self, tick):
        pass


class Harvesting(State):
    id = "Harvesting"
    type = State.Harvesting

    # Harvesting
    harvest_interval = .5 * Config.FRAME_MULTIPLIER
    harvest_timer = 0
    harvest_iterator = 1

    RETURN = 0
    HARVEST = 1

    harvest_target = None

    def __init__(self, unit):
        super().__init__(unit)

    def stage_goto_harvest(self):
        tiles = self.Map.AdjacentMap.adjacent_walkable(*self.harvest_target, 1)

        if not tiles:
            return
        tile = self.unit.shortest_distance(tiles)

        if not self.has_next_state(State.Harvesting):
            self.add_next(State.new(self.unit, State.Harvesting, {
                'harvest_target': self.harvest_target
            }), 1)
            self.unit.move(*tile)

    def stage_harvest(self):
        if self.harvest_timer >= self.harvest_interval:

            tile_data = self.Map.get_tile(*self.harvest_target)

            if self.unit.inventory_lumber + \
                    self.unit.inventory_gold + \
                    self.unit.inventory_oil >= self.unit.inventory_space:
                # Inventory is full, return to base
                recall_building = self.unit.closest_recall_building()
                if not recall_building:
                    # Transition to next state since nothing can be done
                    self.transition()
                    return
                else:
                    self.harvest_iterator = Harvesting.RETURN
                    return

            self.unit.inventory_gold += tile_data['gold_yield']
            self.unit.inventory_lumber += tile_data['lumber_yield']
            self.unit.inventory_oil += tile_data['oil_yield']

            # Todo, adding statistics here becaue its thought to be better for RL????
            self.player.statistics['gold_count'] += tile_data['gold_yield']
            self.player.statistics['lumber_count'] += tile_data['lumber_yield']
            self.player.statistics['oil_count'] += tile_data['oil_yield']

            self.harvest_timer = 0

    def stage_recall(self, recall_building):
        # Must walk
        tiles = self.Map.AdjacentMap.adjacent_walkable(recall_building.x + recall_building.dimension,
                                                       recall_building.y + recall_building.dimension,
                                                       recall_building.dimension + 1)

        if tiles:
            # Wait for free spot to deliver
            tile = self.unit.shortest_distance(tiles)
            if not self.has_next_state(State.Harvesting):
                self.add_next(State.new(self.unit, State.Harvesting, {
                    'harvest_iterator': Harvesting.RETURN,
                    'harvest_target': self.harvest_target
                }), 1)
                self.unit.move(*tile)

    def stage_deliver(self):
        self.unit.player.gold += self.unit.inventory_gold
        self.unit.player.lumber += self.unit.inventory_lumber
        self.unit.player.oil += self.unit.inventory_oil
        self.unit.inventory_gold = 0
        self.unit.inventory_lumber = 0
        self.unit.inventory_oil = 0

    def update(self, tick):
        self.harvest_timer += tick

        if self.harvest_iterator == Harvesting.HARVEST:

            distance = self.unit.distance(*self.harvest_target)
            if distance > 1:
                """
                # Unit cannot reach the harvest tile, It therefore must trigger a Walking state to get to the tile
                """
                self.stage_goto_harvest()
            else:
                """
                # Unit can reach the harvest tile. Start harvesting
                """
                self.stage_harvest()

        elif self.harvest_iterator == Harvesting.RETURN:
            """
            # Find closest recall building (Typically a town-center or lumber mill
            """
            recall_building = self.unit.closest_recall_building()
            if not recall_building or not recall_building.x:
                self.transition()
            distance = self.unit.distance(
                recall_building.x,
                recall_building.y,
                recall_building.dimension
            )

            if distance > 1:
                """
                # Unit cannot reach the recall building, This means that the unit must move towards it
                # Triggers Walking state
                """
                self.stage_recall(recall_building)
            else:
                """
                # Unit are ready for delivering goods to the selected recall building
                # When delivery is done, trigger new harvesting round
                """
                self.stage_deliver()
                self.unit.harvest(*self.harvest_target)


class Building(State):
    id = "Building"
    target = None
    entity = None  # Set by init()
    type = State.Building

    build_timer = 20
    build_duration = 20

    def __init__(self, unit):
        super().__init__(unit)

    def init(self):
        self.build_timer = 0
        self.build_duration = self.target.build_duration
        # Create initial instance of unit
        entity = self.target(self.unit.player)
        entity.add_to_game()
        if not self.unit.structure:
            entity.x = self.unit.x - entity.dimension
            entity.y = self.unit.y - entity.dimension
            self.unit.despawn()
            entity.spawn()

        self.entity = entity

    def spawn_subject(self, unit1, unit2):
        """
        This functions spawns a unit (unit1) given unit2's position
        :param unit1:
        :param unit2:
        :return: Returns True if successful
        """
        adjacent_tiles = self.game.Map.AdjacentMap.adjacent_walkable(
            unit1.x + unit1.dimension,
            unit1.y + unit1.dimension,
            unit1.dimension + 1
        )

        if not adjacent_tiles:
            return False

        unit2.x,unit2.y = adjacent_tiles.pop()
        unit2.spawn()

        return True


    def update(self, tick):
        self.build_timer += tick

        if self.build_timer >= self.build_duration:
            # Build is done
            self.entity.direction = UnitC.UP

            did_spawn = False
            if not self.unit.spawned:
                # Builder is despawned, Spawn unit adjacent to newly built entity
                did_spawn = self.spawn_subject(self.entity, self.unit)
            elif not self.entity.spawned:
                # Entity is despawned, Spawn entity (for example TownHall spawns Peasant)
                did_spawn = self.spawn_subject(self.unit, self.entity)


            if did_spawn:
                self.entity.state.transition()
                self.transition()



# Create mapping
State.mapping = {
    State.Idle: Idle,
    State.Building: Building,
    State.Harvesting: Harvesting,
    State.Combat: Combat,
    State.Walking: Walking,
    State.Dead: Dead,
    State.Spawning: Spawning,
    State.Despawned: Despawned
}
