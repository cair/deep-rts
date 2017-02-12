import time

from ai.algorithms.GreedyMCTS import Policies
from ai.algorithms.GreedyMCTS.MCTS import MCTS
from game.WarC2 import Game

g = Game.start(2, None)
s = g.save()

algorithm = MCTS(
    state=s,
    player=g.players[0],
    policy_expand=Policies.policy_random,
    policy_rollout=Policies.policy_random
)




# 1. Create root of tree
# 2. Forever loop
# 3. Find all available actions for current state S
# 4. For each available action, create leaf node for CURRENT
# 5. Select node based on distribution (MAB / epsilon greedy / roulette wheel selection)
# 6. Do action in game
#
# 7. Start on 2