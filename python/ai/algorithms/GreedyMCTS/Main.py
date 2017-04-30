import random
import time
from threading import Thread
from ai.algorithms.GreedyMCTS.MCTSAction import MCTSAction
from ai.algorithms.GreedyMCTS import Policies
from ai.algorithms.GreedyMCTS.MCTS import MCTS
from game.WarC2 import Game

g = Game.start(2, None)
g.set_gui()


def random_ai():
    actions = MCTSAction.available_actions(g.players[1])
    if actions:
        action = random.choice(actions)
        do_action(action, g)


def do_action(action, gs):
    action_str, uid, action_type, params = action
    unit = gs.units[uid]

    if action_type == 2:
        unit.build(*params)
    elif action_type == 3:
        unit.void()
    elif action_type == 0:
        unit.right_click(*params)
    else:
        # print(action_str, action)
        exit("Fuckdidoodli This should happen")


def on_action(action):
    print(action)
    do_action(action, g)

    rnd_act = random_ai()

def mcts():
    algorithm = MCTS(
        game=g,
        player=g.players[0],
        policy_expand=Policies.policy_random,
        policy_rollout=Policies.policy_random,
        on_action=on_action
    )
t1 = Thread(target=mcts)
t1.start()






now = time.time()

_gui_interval = .05
_update_interval = .12

_gui_next = now + _gui_interval
_update_next = now + _update_interval

while True:
    now = time.time()

    if now > _gui_next:
        g.render()
        _gui_next += _gui_interval

    if now > _update_next:
        g.clock.tick()
        _update_next += _update_interval



# 1. Create root of tree
# 2. Forever loop
# 3. Find all available actions for current state S
# 4. For each available action, create leaf node for CURRENT
# 5. Select node based on distribution (MAB / epsilon greedy / roulette wheel selection)
# 6. Do action in game
#
# 7. Start on 2