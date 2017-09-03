import logging

from game import const
from game import Config
from game.api.Interface import Interface
from game.api.Event import Event, DummyEvent
from game.logic.UnitManager import UnitManager

class Player:

    def __init__(self, game, _id, name="[NO-NAME]", ai_instance=False):
        self.id = _id

        # References
        self.game = game
        self.Map = game.Map
        self.UnitManager = UnitManager

        if not ai_instance:
            self.Event = Event(self)
            self.Interface = Interface(self, self.Event)
            self.Interface.start()
        else:
            self.Event = DummyEvent

        # Final Variables
        self.name = name
        self.race = const.Race.TYPE_HUMAN

        # Variables
        self.statistics = None
        self.units = None
        self.vision = None
        self.defeated = False

        # Game Variables
        #self.lumber = 0
        #self.gold = 0
        #self.oil = 0
        #self.food = 0
        #self.consumed_food = 0

        #self.reset()

        logging.debug("Created player %s" % name)

    def reset(self):
        self.defeated = False
        self.units = []
        self.vision = self.game.Map.base_vision[:]
        self.statistics = {
            'kill_count': 0,
            'death_count': 0,
            'unit_count': 0,
            'gold_count': 0,
            'lumber_count': 0,
            'oil_count': 0
        }
        self.lumber = 1000
        self.gold = 2000
        self.oil = 1000
        self.food = 1
        self.consumed_food = 0

        self.spawn()
        self.Event.notify_start({
            'player_id': self.id,
            'worker': self.units[0]
        })

    def toJSON(self):
        return {
            'units': self.units,
            'statistics': self.statistics,
            'name': self.name,
            'race': self.race,
            'defeated': self.defeated,
            'id': self.id,
            'lumber': self.lumber,
            'gold': self.gold,
            'oil': self.oil,
            'food': self.food,
            'consumed_food': self.consumed_food
        }

    def load(self, data):

        self.units = [x for x in data['units']]
        self.statistics = {
            'kill_count': data['statistics']['kill_count'],
            'death_count': data['statistics']['death_count'],
            'unit_count': data['statistics']['unit_count'],
            'gold_count': data['statistics']['gold_count'],
            'lumber_count': data['statistics']['lumber_count'],
            'oil_count': data['statistics']['oil_count']
        }
        self.name = data['name']
        self.race = data['race']
        self.vision = self.game.Map.base_vision[:]
        self.defeated = data['defeated']
        self.id = data['id']
        self.lumber = data['lumber']
        self.gold = data['gold']
        self.oil = data['oil']
        self.food = data['food']
        self.consumed_food = data['consumed_food']

    def calculate_defeat(self):
        has_units = True if not self.units else False
        self.defeated = has_units

    def get_units(self):
        return [self.game.units[x] for x in self.units]

    def process(self, dt):

        for unit_id in self.units:
            self.game.units[unit_id].process(dt)


        if not Config.NO_FOG:
            self.calculate_fow()

    def spawn(self):
        # Get single spawn tile
        spawn_tile = self.Map.get_spawn_tile(self.id)

        # Create new unit
        unit = UnitManager.worker(self)      # 1. Create a worker
        unit.set_position(*spawn_tile)       # 2. Set worker position
        unit.add_to_game()                   # 3. Add worker to game
        unit.spawn()                         # 4. Spawn worker
        unit.state.transition()              # 5. Transition from Spawning to Idle state

        if Config.MECHANICS_TOWN_HALL_ON_START:
            unit.build(0, force_allow=True)

        logging.debug("Player %s spawned at %s with 1 worker", self.name, spawn_tile)

    def calculate_fow(self):
        self.vision = []

        for unit_id in self.units:
            unit = self.game.units[unit_id]
            if not unit.d['x']:
                continue

            range_x = range(unit.d['x'] - 2 + unit.dimension, min(unit.d['x'] + 3 + unit.dimension, self.Map.height))
            range_y = range(unit.d['y'] - 2 + unit.dimension, min(unit.d['y'] + 3 + unit.dimension, self.Map.width))
            for x in range_x:
                for y in range_y:
                    self.vision.append((x, y))

    def right_click(self, x, y, unit_id):
        self.game.units[unit_id].right_click(x, y)


    def left_click(self, x, y, unit_id):
        pass


    def score_unit(self):
        structures = [type(self.game.units[s]) for s in self.units if self.game.units[s].structure == const.Unit.TYPE_UNIT]
        counter = {}
        sum = 0
        for unit in structures:
            if unit not in counter: counter[unit] = 0                   # Create a counter entry for this unit type
            counter[unit] += 1                                          # Increment counter
            g_s = (unit.cost_gold * Config.GAME_GOLD_MODIFIER)          # Calculate gold value
            l_s = (unit.cost_lumber * Config.GAME_LUMBER_MODIFIER)      # Calculate lumber value
            a = (g_s + l_s) * .15                                       # The score with .20 is resource score
            dx = a * .05                                               # Punishment value (Many buildings are not good)
            score = max(0, a + (a - (counter[unit] * dx)))              # Exponential Smoothing, check wiki
            sum += score

        return sum

    def score_structure(self):
        structures = [type(self.game.units[s]) for s in self.units if self.game.units[s].structure == const.Unit.TYPE_STRUCTURE]
        counter = {}
        sum = 0
        for unit in structures:
            if unit not in counter: counter[unit] = 0                   # Create a counter entry for this unit type
            counter[unit] += 1                                          # Increment counter
            g_s = (unit.cost_gold * Config.GAME_GOLD_MODIFIER)          # Calculate gold value
            l_s = (unit.cost_lumber * Config.GAME_LUMBER_MODIFIER)      # Calculate lumber value
            a = (g_s + l_s) * .15                                       # The score with .20 is resource score
            dx = a * .10                                               # Punishment value (Many buildings are not good)
            score = max(0, a + (a - (counter[unit] * dx)))              # Exponential Smoothing, check wiki
            sum += score

        return sum

    def score_resources(self):
        oil = self.oil * Config.GAME_OIL_MODIFIER
        lumber = self.lumber * Config.GAME_LUMBER_MODIFIER
        gold = self.gold * Config.GAME_GOLD_MODIFIER

        for u in self.units:
            unit = self.game.units[u]
            oil += unit.d['inventory_oil']
            gold += unit.d['inventory_gold']
            lumber += unit.d['inventory_lumber']

        return (oil + lumber + gold) * .20

    def score_military(self):
        military_units = [type(self.game.units[s]) for s in self.units if self.game.units[s].military]
        counter = {}
        sum = 0
        for unit in military_units:
            if unit not in counter: counter[unit] = 0                   # Create a counter entry for this unit type
            counter[unit] += 1                                          # Increment counter
            g_s = (unit.cost_gold * Config.GAME_GOLD_MODIFIER)          # Calculate gold value
            l_s = (unit.cost_lumber * Config.GAME_LUMBER_MODIFIER)      # Calculate lumber value
            a = (g_s + l_s) * .20                                       # The score with .20 is military score
            dx = a * .05                                                # Punishment value (Big armies are not good)
            score = max(0, a + (a - (counter[unit] * dx)))              # Exponential Smoothing, check wiki
            sum += score


        return sum

    def score_defence(self):
        return 0

    def score_total(self):
        return self.score_military() + self.score_resources() + self.score_structure() + self.score_unit() + self.score_defence()










