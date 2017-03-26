import time

from ai.algorithms.Algorithm import Algorithm


class GreedyMCTS(Algorithm):
    initial_epsilon_0 = float(0.2)
    initial_epsilon_l = float(0.25)
    initial_epsilon_g = float(0.0)

    MAX_DEPTH = 10

    def __init__(self, action):
        super().__init__(action)

        self.epsilon_0 = 0.2
        self.epsilon_l = 0.25
        self.epsilon_g = 0.0
        self.strategy = Node.STRAT_E_GREEDY

        self.tree = None

    def ready(self):
        return True

    def compute(self):
        action = self.Action

        g = action.player.game
        max_p = action.player.id
        min_p = next(p.id for p in action.player.game.players if p.id != max_p)

        self.tree = Node(max_p, min_p, g, {}, None, 2.0, 0)  # Eval function is score somehow

        self.expand(max_p, min_p)

        exit()

    TIME_BUDGET = 2

    # BUDGET_TICKS = 1000

    def expand(self, max_p, min_p):
        start = time.time()
        end = None
        iterations = 0

        while True:
            # DO Expansion
            #NaiveMCTSNode leaf = tree.selectLeaf(
            #    player,
            #    1-player, epsilon_l, epsilon_g, epsilon_0, global_strategy, MAX_TREE_DEPTH, current_iteration++);

            leaf = self.tree.leaf(
                max_p,
                min_p,
                self.epsilon_l, self.epsilon_g, self.epsilon_0,
                self.strategy,
                GreedyMCTS.MAX_DEPTH, iterations)

            print(leaf)

            iterations += 1
            end = time.time()

            if GreedyMCTS.TIME_BUDGET >= 0 and not end - start < GreedyMCTS.TIME_BUDGET:
                break
