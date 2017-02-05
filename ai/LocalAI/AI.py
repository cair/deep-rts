from Mechanics.API.LocalAI import LocalAI
from ai.LocalAI.ChainCommand import ChainCommand
from ai.LocalAI.WierdMCTS.MCTS import MCTS


class AIInstance:

    def __init__(self, ai):
        self.ai = ai
        self.player = ai.player
        self.Event = ai.event
        self.Action = ai.action
        self.command = self.construct()

        self.Event.on_victory(self.on_victory)
        self.Event.on_defeat(self.on_defeat)
        self.Event.on_event(self.on_event)
        self.Event.on_frame(self.on_frame)

        self.tick = 0

        self.mcts = MCTS(self.ai)



    def construct(self):

        # Build AI
        command = ChainCommand(self.Action)

        command.then_idle_worker(0)
        command.then_build_town_hall(0)

        command.then_idle_worker((8 * 255) + 1)
        command.then_wood(0)
        command.then_wood_count(0, 250)
        command.then_gold(0)
        command.then_gold_count(0, 500)

        command.then_worker(0)
        command.then_build_farm(0)

        command.then_idle_worker((8 * 10000) + 1)
        command.then_gold(0)

        # Build new worker when enough gold
        command.then_gold_count(0, 400)
        command.then_idle_town_hall(0)
        command.then_build_worker(0)

        # New worker harvest gold
        command.then_idle_worker((8 * 60) + 1)
        command.then_wood(0)
        command.then_wood_count(0, 250)
        command.then_gold(0)
        command.then_gold_count(0, 500)

        command.then_worker(0)
        command.then_build_farm(0)
        command.then_gold(0)

        for i in range(6):
            g_w = i % 2 == 0

            # Build new worker when enough gold
            command.then_gold_count(0, 400)
            command.then_idle_town_hall(0)
            command.then_build_worker(0)

            # New worker harvest_ gold
            command.then_idle_worker((8 * 60) + 1)
            command.then_gold(0) if g_w else command.then_wood(0)


        command.then_idle_worker((8 * 60) + 1)
        command.then_wood(0)

        # Build barracks next
        command.then_wood_count(0, 450)
        command.then_gold_count(0, 700)
        command.then_worker(0)
        command.then_build_barracks(0)

        command.then_idle_worker((8 * 60) + 1)
        command.then_wood(0)

        for i in range(2):
            command.then_idle_barracks(0)
            command.then_build_footman(0)

        command.then_pause(0)
        return command

    def on_frame(self, tick):

        if self.player.id == 2:
            return

        """if event_type == 0:
            self.tick += 1
            if self.command.eligible(self.tick):
                self.command.run()"""


        move = self.mcts.next_move()

        if move:
            move.execute()


    def on_event(self, event_type, data):
        pass

    def on_defeat(self, data):
        pass

    def on_victory(self, data):
        pass






"""
Game hook
"""
instances = []
def on_hook(local):
    ai = AIInstance(local)
    print("Created hook on player %s" % local.player.id)


# Append hook to Local AI endpoint
LocalAI.hooks.append(on_hook)

# Import game (Starts it)
from Main import game

game.loop()


print("Player %s won the game!" % game.winner)
