"""
 self.actions = {
                'value': [0.0 for _ in range(len(min_actions))],
                'visits': [0 for _ in range(len(min_actions))],
                'action_count': len(min_actions),
                'actions': min_actions
            }
"""


class Node:
    STRAT_E_GREEDY = 0
    STRAT_UCB1 = 1

    def __init__(self, actions, game, parent, eval_bound, iteration):
        self.depth = parent.depth if parent else 0
        self.eval_bound = eval_bound  # 1.0f
        self.iteration = iteration
        self.parent = parent

        # Add
        try:
            self.parent.children.append(self)
        except AttributeError:
            pass


        self.children = []
