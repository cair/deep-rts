import random
from game.const import Unit

class ChainCommand:

    def __init__(self, Action):
        self.Action = Action
        self.chain = []
        self.current = 0
        self.last_tick = 0

        self.reg1 = None
        self.reg2 = None

    def then_idle_worker(self, tick_delay, insert_index=None):
        ele = (self.idle_worker, tick_delay, [])
        if insert_index is not None:
            self.chain.insert(insert_index, ele)
        else:
            self.chain.append(ele)

    def then_build_town_hall(self, tick_delay):
        self.chain.append((self.build_town_hall, tick_delay, []))

    def then_build_farm(self, tick_delay):
        self.chain.append((self.build_farm, tick_delay, []))

    def then_build_barracks(self, tick_delay):
        self.chain.append((self.build_barracks, tick_delay, []))

    def then_build_worker(self, tick_delay):
        self.chain.append((self.build_worker, tick_delay, []))

    def then_build_footman(self, tick_delay):
        self.chain.append((self.build_footman, tick_delay, []))

    def then_idle_town_hall(self, tick_delay):
        self.chain.append((self.idle_town_hall, tick_delay, []))

    def then_idle_barracks(self, tick_delay):
        self.chain.append((self.idle_barracks, tick_delay, []))

    def then_gold(self, tick_delay):
        self.chain.append((self.gold, tick_delay, []))

    def then_wood(self, tick_delay):
        self.chain.append((self.wood, tick_delay, []))

    def then_pause(self, tick_delay):
        self.chain.append((self.pause, tick_delay, []))

    def then_wood_count(self, tick_delay, amount):
        self.chain.append((self.wood_count, tick_delay, [amount]))

    def then_gold_count(self, tick_delay, amount):
        self.chain.append((self.gold_count, tick_delay, [amount]))

    def then_worker(self, tick_delay):
        self.chain.append((self.worker, tick_delay, []))

    def gold(self):
        worker = self.reg1
        gold_actions = self.Action.harvest_gold_actions(worker)
        action = random.choice(gold_actions)
        action[0](*action[1])
        print("Harvest Gold!")
        return True

    def wood(self):
        worker = self.reg1
        wood_actions = self.Action.harvest_wood_actions(worker)
        action = random.choice(wood_actions)
        action[0](*action[1])
        print("Harvest Wood!")
        return True

    def gold_count(self, amount):

        current_gold = self.Action.player.gold

        if current_gold >= amount:
            print("Gold count: %s (%s/%s)" % (current_gold, current_gold, amount))
            return True
        return False

    def wood_count(self, amount):
        current_wood = self.Action.player.lumber

        if current_wood >= amount:
            print("Wood count: %s (%s/%s)" % (current_wood, current_wood, amount))
            return True

        return False

    def idle_town_hall(self):

        town_halls = self.Action.idle_units(Unit.TOWN_HALL)
        print("Idle Town-Hall")
        if town_halls:
            self.reg1 = town_halls.pop()

            return True
        else:
            return False

    def idle_worker(self):
        idle_worker = self.Action.idle_worker()
        self.reg1 = idle_worker
        print("Idle Worker")
        if not self.reg1:
            self.then_idle_worker(56, self.current)
            return False
        return True

    def idle_barracks(self):
        idle_barracks = self.Action.idle_barracks()
        print("Idle Barracks")
        if idle_barracks:
            self.reg1 = idle_barracks.pop()
        else:
            return False
        return True

    def worker(self):
        workers = self.Action.workers()

        if workers:
            self.reg1 = workers.pop()
            return True
        return False

    def pause(self):
        return False

    def build_town_hall(self):
        assert self.reg1.id == Unit.PEASANT
        method = getattr(self.reg1, 'build')
        method(0)
        print("Build Town-Hall")
        return True

    def build_farm(self):
        assert self.reg1.id == Unit.PEASANT
        self.reg1.state.clear_next()
        method = getattr(self.reg1, 'build')
        method(1)
        print("Build Farm")
        return True

    def build_barracks(self):
        assert self.reg1.id == Unit.PEASANT
        self.reg1.state.clear_next()
        method = getattr(self.reg1, 'build')
        print("Build Barracks")
        if method(2):
            return True
        return False

    def build_worker(self):
        assert self.reg1.id == Unit.TOWN_HALL
        print("Build Worker")
        if self.reg1.build(0):
            return True
        return False

    def build_footman(self):
        assert self.reg1.id == Unit.BARRACKS
        print("Build Footman")
        if self.reg1.build(0):
            return True
        return False

    def eligible(self, tick):
        if self.current >= len(self.chain):
            return False
        current = self.chain[self.current]
        if tick > current[1] + self.last_tick:
            self.last_tick = current[1] + self.last_tick
            return True
        return False

    def run(self):
        current = self.chain[self.current]
        if current[0](*current[2]):
            self.current += 1

    def has_element(self, store):
        return True if store else False

    def not_none(self, store):
        return True if store is not None else False

    def _add(self, data):
        self.chain.append(data)