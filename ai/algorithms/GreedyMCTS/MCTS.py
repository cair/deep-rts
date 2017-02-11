from ai.algorithms.GreedyMCTS.MCTSAction import MCTSAction
from game.WarC2 import Game
import numpy as np

from game.graphics.GUI import GUI


class MCTS:
    OPTIONS_GUI = True


    epsilon_0 = 0.75


    def __init__(self, state, player, policy_expand, policy_rollout):
        self.state = state                                  # Inital game state
        self.player = player                                # Max player
        self.policy_expand = policy_expand                  # Policy for selecting states
        self.policy_rollout = policy_rollout                # Policy for selecting actions when expanding tree




        # Create a game instance (TODO)
        gs = Game.load(False, state)                            # Create a game instance
        self.gui = GUI(gs, None)

        self.tree = MCTSNode(gs.save(), self.player.id, None, 0)   # Create tree root node
        self.current = self.tree


        while True:
            self.process()


            # GUI for debug purposes
            self.gui.game = self.current.gs
            self.gui.player = self.current.max_p
            self.Map = self.gui.game.Map
            self.gui.render(0)


    def process(self):

        if self.current.children: #and np.random.uniform() >= MCTS.epsilon_0:         # Selection of node
            node = self.policy_expand(self.current.children)                          # Select node with expand policy
            self.current = node                                                       # Set node as current node
        else:
            # Selection of action
            # Select action with rollout policy
            # Compute state
            action = self.policy_rollout(self.current.actions)
            print(action)
            self.do_action(action, self.current)
            self.current = MCTSNode(self.current.gs.save(), self.player.id, self.current, self.current.depth + 1)
            self.current.parent.children.append(self.current)


    def do_action(self, action, node):
        action_str, uid, action_type, params = action
        unit = node.gs.units[uid]

        if action_type == MCTSAction.BUILD:
            unit.build(*params)
        elif action_type == MCTSAction.VOID:
            unit.void()
        elif action_type == MCTSAction.RIGHT_CLICK:
            unit.right_click(*params)
        else:
            print(action_str, action)
            exit("Fuckdidoodli This should happen")




class MCTSNode:
    """
    # A MCTSNode is representing a gamestate. This gamestate is a result of a action, done by the algorithm (policy)
    # An action can be thought as an edge
    """
    def __init__(self, gs_fork, max_id, parent, depth):
        self.edge = None            # Which action that led to this state (Parent ---Action--- This)
        self.children = list()      # Which states derived from this state
        self.actions = list()       # Which actions can be explored from this state
        self.parent = parent        # Which state this state was derived from
        self.depth = depth          # Which depth this node are on


        self.gs = Game.load(False, gs_fork, False)                        # Fork a new gamestate
        max_p = next(p for p in self.gs.players if p.id == max_id)        # Find max player given input id
        min_p = next(p for p in self.gs.players if p != max_p)            # Find min player given max player

        self.max_p = max_p

        max_actions = MCTSAction.available_actions(max_p)
        min_actions = MCTSAction.available_actions(min_p)


        while not self.gs.winner and not max_actions and not min_actions: # While there is no winner
                                                                    # and no available actions of max
                                                                    # and no available actions for min
            self.gs.clock.tick()                                    # Tick game clock
            max_actions = MCTSAction.available_actions(max_p)       # Check for new available actions for max
            min_actions = MCTSAction.available_actions(min_p)       # Check for new available actions for min


        if self.gs.winner:                                                # If there is a winner
            exit("Game over!")                                      # Gameover!


        self.actions = max_actions if max_actions else min_actions  # Action set is max_p's if not we select min_p

        print(self.depth, max_p.score_total(), len(max_actions), len(min_actions), max_actions, min_actions)
