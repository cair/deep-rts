import time
from threading import Thread

from game import Config
from game.Game import Game
import numpy as np

from ai.algorithms.GreedyMCTS.MCTSAction import MCTSAction


class MCTS:
    OPTIONS_GUI = True
    DEPTH = 10
    epsilon_0 = 0.40


    def __init__(self, game, player, policy_expand, policy_rollout, on_action):
        self.player = player  # Max player
        self.policy_expand = policy_expand  # Policy for selecting states
        self.policy_rollout = policy_rollout  # Policy for selecting actions when expanding tree

        self.runs = 0
        self.iteration = 0


        while True:
            orginal_state = game.save()
            gs = Game.load(False, game.save(), ai_instance=True)  # Create a game instance
            tree = MCTSNode(gs, self.player.id, None, 0, None)  # Create tree root node

            best_move = self.get_best_move(tree, orginal_state)
            on_action(best_move)

            print("[Runs=%s, Iterations=%s]" % (
                self.runs,
                self.iteration))

            self.iteration = 0
            self.runs = 0

    def get_best_move(self, tree, orginal_state):

        current = tree

        now = time.time()
        time_budget = now + .1
        while now < time_budget:
            now = time.time()

            if current.children and np.random.uniform() <= MCTS.epsilon_0 and False:  # Selection of node
                node = self.policy_expand(current.children)  # Select node with expand policy
                self.do_action(node.edge, node)  # Do the action from to this node
                current = node  # Set node as current node
            else:
                # Selection of action
                # Select action with rollout policy
                # Compute state
                action = self.policy_rollout(current.actions)  # Select action based on policy
                self.do_action(action, current.gs)  # Do the action
                current = MCTSNode(current.gs, self.player.id, current, current.depth + 1, action)
                current.parent.children.append(current)  # Add children to parent node

            self.iteration += 1  # DEBUG, Iteration counter
            if current.depth == MCTS.DEPTH or current.gs.winner:  # If algorithm has reached depth limit
                self.runs += 1  # DEBUG, Run counter

                accumulated_score = 0  # Create accumulation varable
                while current.parent is not None:  # Propogate up the tree
                    accumulated_score += current.dscore  # Add delta score to accumulation
                    current.parent.dscore += accumulated_score
                    current = current.parent

                current.fork(orginal_state)  # Fork new gamestate

        min = 0
        max = 0
        max_action = None
        for ch in tree.children:
            if ch.score > max:
                max_action = ch.edge
                max = ch.score


        return max_action


    def do_action(self, action, gs):
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


class MCTSNode:
    I_COUNTER = 0
    """
    # A MCTSNode is representing a gamestate. This gamestate is a result of a action, done by the algorithm (policy)
    # An action can be thought as an edge
    """

    def __init__(self, gs, max_id, parent, depth, action):
        self.id = MCTSNode.I_COUNTER
        MCTSNode.I_COUNTER += 1
        self.edge = action  # Which action that led to this state (Parent ---Action--- This)
        self.children = list()  # Which states derived from this state
        self.actions = list()  # Which actions can be explored from this state
        self.parent = parent  # Which state this state was derived from
        self.depth = depth  # Which depth this node are on

        self.gs = gs  # Fork a new gamestate
        max_p = next(p for p in self.gs.players if p.id == max_id)  # Find max player given input id
        min_p = next(p for p in self.gs.players if p != max_p)  # Find min player given max player

        self.max_p = max_p

        self.score = self.max_p.score_total()
        if self.parent:
            self.dscore = self.score - self.parent.score
        else:
            self.dscore = 0

        while True:
            self.gs.process(1)

            if self.gs.winner:  # If there is a winner
                break

            max_actions = MCTSAction.available_actions(max_p)  # Check for new available actions for max
            if max_actions:  # If no winner and no max_action
                self.actions = max_actions
                break

            min_actions = MCTSAction.available_actions(min_p)  # Check for new available actions for min
            if min_actions:
                self.actions = min_actions
                break

    def fork(self, orginal_gs):
        self.gs = Game.load(False, orginal_gs)
