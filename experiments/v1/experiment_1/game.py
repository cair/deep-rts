from multiprocessing import Process
from DeepRTS.pyDeepRTS import DeepRTS
from DeepRTS import ensure_data_directory
from threading import Thread
import time

from data_client import DataClient
from v1.experiment_1 import util
from .agent import Agent
from .config import *

# Ensure data directory
ensure_data_directory()


class Game(Process):

    def __init__(self, gui=False):
        Process.__init__(self)
        self.id = util.haiku()
        self.game = DeepRTS(2, True)
        self.time_based = False
        self.tick_interval = 15
        self.tick_next = 0
        self.game_max_length = 10 * 60 * 10 # 10 ticks * 60 seconds * 10 minutes
        if self.time_based:
            self.game.setFPS(fps * 2)
            self.game.setUPS(ups)
        else:
            self.game.setFPS(60)
            self.game.setUPS(300)
        self.gui = gui
        self.state_size = self.game.getState().shape
        self.agent = Agent(self.state_size, 16)
        self.data_client = DataClient(game_id=self.id, agent=self.agent)

        self.players = [self.game.addPlayer() for _ in range(players)]
        self.player = self.players[0]

        if gui:
            self.game.initGUI()

        self.game.start()

        if self.time_based:
            self.game_loop = Thread(target=self.loop)

        self.running = True

    def run(self):
        if self.time_based:
            self.game_loop.start()

        last_score = self.player.getScore()

        while True:

            if not self.time_based:
                # Use while not time_based
                self.game.tick()  # Updates Game Internal Clock
                self.game.update()  # Updates the game state
                if self.gui:
                    self.game.render()  # Renders the game
                self.game.caption()  # Updates statistics

                if self.game.ticks < self.tick_next:
                    continue
                self.tick_next += self.tick_interval

            # # Observe
            # s0 = Current State
            # # Do Action
            # a = dqn.getAction(s0)
            # # Observe + Feedback
            # r, s1, t = p.getScore(), g.getState(), g.isTerminal()
            # # Save memory & Train network
            # dqn.train()
            s0 = self.game.getState()
            a = self.agent.act(s0)
            self.player.queueAction(a)

            if self.time_based:
                time.sleep(1)

            current_score = self.player.getScore()
            r = abs(last_score - current_score)
            last_score = current_score
            t = self.game.ticks >= self.game_max_length or self.game.checkTerminal()
            s1 = self.game.getState()




            if t:
                victory = self.player.checkDefeat()
                if victory:
                    # Player won
                    r *= 10

                self.data_client.post_result(last_score, not victory)
                self.data_client.update_weights()
                print("Terminal: id:%s ticks:%s" % (self.id, self.game.ticks))
                self.game.reset()
                self.tick_next = 0

            self.data_client.observe(s0, a, r, t, s1)

    def loop(self):

        next_update = time.time()
        step_size = 1.0 / ups

        while self.running:
            now = time.time()
            sleep_time = max(0, next_update - now)

            time.sleep(sleep_time)

            #if now < next_update:
            #    continue

            self.game.tick()  # Updates Game Internal Clock
            self.game.update()  # Updates the game state
            if self.gui:
                self.game.render()  # Renders the game
            self.game.caption()  # Updates statistics

            next_update = now + step_size



