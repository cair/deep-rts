from ai.LocalAI.ChainCommand import ChainCommand
from ai.algorithms.Algorithm import Algorithm


class Hardcode_Simple1(Algorithm):

    def __init__(self, action):
        super().__init__(action)

        self.command = self.build()

    def build(self):
        command = ChainCommand(self.Action)

        # Build Town_Hall
        command.then_idle_worker(0)
        command.then_build_town_hall(255 * 8)

        # Farm 250 wood
        command.then_idle_worker(0)
        command.then_wood(0)
        command.then_wood_count(0, 250)

        # Farm 500 gold
        command.then_gold(0)
        command.then_gold_count(0, 500)

        # Build a farm
        command.then_worker(0)
        command.then_build_farm(255 * 8)

        # Get idle worker (The one that just finished of farm)
        command.then_idle_worker((8 * 255))
        command.then_gold(0)

        # Build new worker when enough gold
        command.then_gold_count(0, 400)
        command.then_idle_town_hall(0)
        command.then_build_worker(8  * 60)

        # New Worker harvest gold
        command.then_idle_worker((8 * 60) + 1)
        command.then_wood(0)
        command.then_wood_count(0, 250)
        command.then_gold(0)
        command.then_gold_count(0, 500)

        # Build farm
        command.then_worker(0)
        command.then_build_farm(255 * 8)
        command.then_gold(0)

        # Create 6 new workers, where 3 of them handles wood and 3 handles gold
        for i in range(6):
            g_w = i % 2 == 0

            # Build new worker when enough gold
            command.then_gold_count(0, 400)
            command.then_idle_town_hall(0)
            command.then_build_worker(8 * 255)

            # New worker harvest_ gold
            command.then_idle_worker(0)
            command.then_gold(0) if g_w else command.then_wood(0)


        # Build barracks next
        command.then_wood_count(0, 450)
        command.then_gold_count(0, 700)
        command.then_worker(0)
        command.then_build_barracks(200 * 8)

        # Make the idle worker gather wood
        command.then_idle_worker(0)
        command.then_wood(0)

        for i in range(2):
            command.then_idle_barracks(0)
            command.then_gold_count(0, 600)
            command.then_build_footman(60 * 8)

        return command

    def ready(self):
        return self.command.eligible(self.tick)

    def compute(self):
        self.command.run()