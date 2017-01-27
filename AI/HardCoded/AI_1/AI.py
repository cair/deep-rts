import time
import random

from Event import Event
from Mechanics.Constants import Unit, Map
from Mechanics.Unit.Farm import Farm
from Mechanics.Unit.Peasant import Peasant
from Mechanics.Util import ArrayUtil


class AI:

    def __init__(self, player):
        self.player = player
        Event.callbacks.append(self.on_feedback)

        self.base_area = []
        self.base_wood_plots = []
        self.base_gold_plots = []

        self._analyze_base()
        self.delay = time.time() + 4
        pass #print("o my god")
        self.ite = 1

    def get_worker(self):
        return next((x for x in self.player.units if x.id == Unit.PEASANT and x.get_state() != Unit.Uncomplete), None)

    def get_idle_peasants(self):
        return (x for x in self.player.units if x.id == Unit.PEASANT and x.get_state() == Unit.Idle)

    def get_idle_peasant(self):
        return next(self.get_idle_peasants(), None)

    def get_town_hall(self):
        return next((x for x in self.player.units if x.id == Unit.TOWN_HALL), None)

    def can_afford_farm(self):
        return self.player.gold > Farm.cost_gold and self.player.lumber > Farm.cost_lumber

    def can_afford_peasant(self):
        return self.player.gold > Peasant.cost_gold

    def harvest_gold(self, peasant):
        peasant.harvest(*random.choice(self.gold_plots))

    def harvest_wood(self, peasant):
        peasant.harvest(*random.choice(self.wood_plots))

    def make_idle_peasants_work(self):
        for peasant in self.get_idle_peasants():
            a = random.randint(0,1)
            self.harvest_gold(peasant) if a == 1 else self.harvest_wood(peasant)

    def _analyze_base(self):
        base = ArrayUtil.get_area(self.player.units[0].x - 5, self.player.units[0].y - 5, 10, 10)
        self.base_area = base

        # Find wood plots
        self.wood_plots = [(x, y) for x, y in base if self.player.map.tiles[x][y] == Map.WOOD]

        # Find gold plots
        self.gold_plots = [(x, y) for x, y in base if self.player.map.tiles[x][y] == Map.GOLD]

    def sequence_intro(self):

        # Build base
        if self.ite == 1:
            print("First")
            idle_worker = self.get_idle_peasant()
            idle_worker.build_town_hall()
            self.ite += 1

        # Go to work
        if self.ite == 2:
            idle_worker = self.get_idle_peasant()
            if not idle_worker:
                return
            self.harvest_gold(idle_worker)
            self.ite += 1

        if self.ite == 3:
            if self.can_afford_peasant():
                self.get_town_hall().build_peasant()
                self.ite += 1

        if self.ite == 4:
            idle_worker = self.get_idle_peasant()
            if idle_worker:
                self.harvest_wood(idle_worker)
                self.ite += 1

        if self.ite == 5:

            # Build farms and workers until 12 food is filled.
            if self.player.food < 18:
                if self.can_afford_farm():
                    peasant = self.get_worker()
                    if peasant and peasant.get_state() != Unit.Building:
                        peasant.end_harvesting()
                        peasant.build_farm()
                        self.ite += 1

            """if self.player.food == self.player.consumed_food:
                return

            if self.player.consumed_food < 12:
                self.make_idle_peasants_work()
                if self.can_afford_peasant():
                    self.get_town_hall().build_peasant()

                return
            """






        if self.ite == 6:
            pass
        if self.ite == 7:
            pass
        if self.ite == 8:
            pass
        if self.ite == 9:
            pass
        if self.ite == 2:
            pass
        if self.ite == 2:
            pass


    def on_feedback(self, event, data=None):

        if event == Event.New_State:
            self.sequence_intro()
        elif event == Event.Player_Spawn:
            pass #print("Player Spawn")
        elif event == Event.Unit_Died:
            pass #print("Unit Died")
        elif event == Event.Retaliate:
            pass #print("Unit Retaliate")
        elif event == Event.Attack:
            pass #print("Unit Attack")
        elif event == Event.Unit_Spawn:
            pass #print("Unit Spawn")
        elif event == Event.Unit_Start_Harvesting:
            pass #print("Unit Start Harvesting")
        elif event == Event.Unit_Stop_Harvesting:
            pass #print("Unit Stop Harvesting")
        elif event == Event.Unit_Set_Position:
            pass # pass #print("Unit Set Position")
        elif event == Event.Unit_Move_Harvestable:
            pass #print("Unit Move Harvestable")
        elif event == Event.Unit_Move:
            pass #print("Unit Move")
        elif event == Event.Unit_Move_Attack:
            pass #print("Unit Move Attack")
        elif event == Event.Unit_Build:
            pass #print("Unit Build")
