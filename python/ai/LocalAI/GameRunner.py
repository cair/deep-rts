from threading import Thread

from ai.LocalAI.EventLoop import EventLoop
from game import Config
from game.ParallellWorker import ParallellWorker
from game.Game import Game
from game.api.LocalAI import LocalAI


class GameRunner(Thread):

    def __init__(self):
        Thread.__init__(self)
        self.g = None

        LocalAI.hooks.append(GameRunner.on_hook)

    def run(self):
        self.g = Game()
        print(self.g)
        self.g.parallell_worker = ParallellWorker(False)
        self.g.loop()

    @staticmethod
    def on_hook(local):
        print("Created hook on player %s" % local.player.id)

        algorithm = None
        #try:
            # Attempt to create AI for player
        algorithm = Config.AI_ALGORITHMS[local.player.id - 1](local.action)
        #except:
        #    print("Could not find algorithm on position %s in Config.AI_ALGORITHMS" % (local.player.id - 1))

        if algorithm:
            e_loop = EventLoop(algorithm, local.player)



