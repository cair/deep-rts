
import multiprocessing
from threading import Thread

from .game import Game
from .agent import Agent

# Spawn game instances
games = []
observe_queue = multiprocessing.Queue()


p = Game(gui=True)
p.start()
games.append(p)
agent = Agent(p.state_size, 16)

for i in range(0):
    p = Game()
    p.start()
    games.append(p)
    print("Starting game instance: %s " % i)


updates = 0
while True:

    while not observe_queue.empty():
        observation = observe_queue.get()
        agent.observe(observation)
        updates += 1

        if updates % 100 == 0:
            agent.replay()
            print("Train: u:%s q:%s" % (updates, observe_queue.qsize()))

        if updates % 5000 == 0:
            for game in games:
                game.agent.brain.set_weights(agent.brain.get_weights())
            print("Weights: u:%s q:%s" % (updates, observe_queue.qsize()))







