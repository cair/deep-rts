from random import random

import numpy as np

from Mechanics.Unit import Unit


class MCTS:

    def __init__(self, ai):
        self.ai = ai
        self.player = ai.player
        self.Event = ai.event
        self.Action = ai.action
        self.counter = 1


        self.moves = [
            'attack_peasant',
            'attack_footman',
            'attack_farm',
            'attack_town',
            #'defend_peasant',
            #'defend_footman',
            #'defend_farm',
            #'defend_town'
            'harvest_gold',
            'harvest_lumber',
            'build_town',
            'build_farm',
            'build_peasant',
            'build_footman',
            'no_action'
        ]

        self.n = len(self.moves)
        self.child_template = [1 for x in range(self.n)]
        self.tree = {
            0: (None, self.child_template.copy(), None)  # [Index, Children, Parent]
        }

        self.avg_length = 0
        self.avg_counter = 0
        self.depth = 0

        self.node_key = 0
        self.node = self.tree[0]

    def selection(self, distribution):
        while True:
            index = int(self.n * np.random.uniform())
            if np.random.uniform() < distribution[index] / np.sum(distribution):
                break
        return index

    def select(self, fs):
        p = np.random.uniform(0, sum(fs))
        for i, f in enumerate(fs):
            if p <= 0:
                break
            p -= f
        return i


    def next_move(self):
        self.counter += 1
        self.avg_counter += 1

        distribution = self.node[1]
        index = self.select(distribution)
        action = self.moves[index]
        #print(action)

        success = self.execute(action)

        # Go to next round, action was valid
        new_node_key = self.counter
        if new_node_key in self.tree:
            new_node = self.tree[new_node_key]
        else:
            new_node = (index, self.child_template.copy(), self.node_key)
            self.tree[new_node_key] = new_node

        self.node_key = new_node_key
        self.node = new_node

        if success:
             pass  #self.reward_path()
        else:
            # Not valid move, Surrender
            print(self.tree[0])

            self.avg_counter = 0


            #self.punish_path()
            parent = self.tree[self.node[2]]
            parent[1][self.node[0]] = 0

            # Set back root
            self.node = self.tree[0]
            self.node_key = 0

            self.player.surrender()
            #print("------DONE-----")

    def reward_path(self):
        # Punish path
        i = 0
        current = self.node
        while True:
            i += 1
            #print(self.moves[current[0]])
            parent = self.tree[current[2]]
            parent[1][current[0]] += 1 / (1 / i)
            current = parent

            if current[2] is None:
                break

    def punish_path(self):
        # Punish path
        i = 0
        current = self.node
        """while True:
            i += 1
            #print(self.moves[current[0]])
            parent = self.tree[current[2]]
            parent[1][current[0]] = max(0, parent[1][current[0]] - 0.05)
            current = parent

            if current[2] is None:
                break
        """





        """while current[2] is not None:
            i += 1
            # Get parent
            parent = self.tree[current[2]]

            # Decrease value
            parent[1][current[0]] = max(0, parent[1][current[0]] - (factor / i))
            path.append(self.moves[current[0]])
            current = parent
        print(list(reversed(path)))"""

    def execute(self, action):
        status = False
        if action == 'harvest_gold':
            status = self.Action.harvest_gold()
        elif action == 'harvest_lumber':
            status = self.Action.harvest_lumber()
        elif action == 'build_peasant':
            status = self.Action.build_peasant()
        elif action == 'build_footman':
            status = self.Action.build_footman()
        elif action == 'build_town':
            status = self.Action.worker_build(0)
        elif action == 'build_farm':
            status = self.Action.worker_build(1)
        elif action == 'build_barracks':
            status = self.Action.worker_build(2)
        elif action == 'attack_peasant':
            status = self.Action.attack(self.player, Unit.PEASANT)
        elif action == 'attack_town':
            status = self.Action.attack(self.player, Unit.TOWN_HALL)
        elif action == 'attack_footman':
            status = self.Action.attack(self.player, Unit.FOOTMAN)
        elif action == 'attack_farm':
            status = self.Action.attack(self.player, Unit.FARM)
        elif action == 'no_action':
            status = True
        return status