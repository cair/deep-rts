
import time

from game import Config
from game.WarC2 import Game
import numpy as np

from ai.algorithms.GreedyMCTS.MCTSAction import MCTSAction


class MCTS:
    OPTIONS_GUI = True
    DEPTH = 10
    epsilon_0 = 0.40

    def __init__(self, state, player, policy_expand, policy_rollout):
        self.state = state                                  # Inital game state
        self.player = player                                # Max player
        self.policy_expand = policy_expand                  # Policy for selecting states
        self.policy_rollout = policy_rollout                # Policy for selecting actions when expanding tree

        gs = Game.load(False, state, ai_instance=True)                            # Create a game instance
        gs_orig = gs.save()
        self.runs = 0
        self.iteration = 0

        self.tree = MCTSNode(Game.load(False, state=gs_orig), self.player.id, None, 0, None)   # Create tree root node
        self.tree.orig_gs = gs_orig                                                             # Save the original game state in root
        self.current = self.tree                                                           # Set root as current node
        self.best_so_far = self.tree

        cdef double _gui_interval = (1.0 / Config.FPS)              # Interval of GUI rerender
        cdef double _gui_next = time.time() + _gui_interval         # Next render of gui
        cdef double _graph_interval = (1.0 / 0.1)                   # Inteval of graph save
        cdef double _graph_next = time.time() + _graph_interval     # Next graph save
        _graph_record = False                           # Weither to save graph or not (PNG)
        cdef double _start_time = time.time()
        while True:
            self.process()

            now = time.time()
            if now > _gui_next:
                self.current.gs.set_gui()
                self.current.gs.gui.render(0)
                print("[Runs=%s, Iterations=%s, IPS=%s, RPS=%s DScore=%s Action=%s]" % (
                    self.runs,
                    self.iteration,
                    int(1/((now-_start_time)/self.iteration)),
                    1/((now-_start_time)/(self.runs+1)),
                    self.best_so_far.dscore,
                    self.best_so_far.edge))
                _gui_next = now + _gui_interval

            if _graph_record and now > _graph_next:
                from reports.MCTSGraph import draw_tree
                draw_tree(self.tree)
                _graph_next += _graph_interval

    def process(self):
        if self.current.children and np.random.uniform() <= MCTS.epsilon_0 and False:       # Selection of node
            node = self.policy_expand(self.current.children)                      # Select node with expand policy
            self.do_action(node.edge, node)                                       # Do the action from to this node
            self.current = node                                                   # Set node as current node
        else:
            # Selection of action
            # Select action with rollout policy
            # Compute state
            action = self.policy_rollout(self.current.actions)                      # Select action based on policy
            self.do_action(action, self.current)                                    # Do the action
            self.current = MCTSNode(self.current.gs, self.player.id, self.current, self.current.depth + 1, action)
            self.current.parent.children.append(self.current)                       # Add children to parent node

        self.iteration += 1                                            # DEBUG, Iteration counter
        if self.current.depth == MCTS.DEPTH or self.current.gs.winner: # If algorithm has reached depth limit
            self.runs += 1                                             # DEBUG, Run counter

            prop_node = self.current                                   # Set propogation node to current leaf
            accumulated_score = 0                                      # Create accumulation varable
            while prop_node.parent != self.tree:                       # Propogate up the tree
                accumulated_score += prop_node.dscore                  # Add delta score to accumulation
                prop_node.dscore += accumulated_score                  # Add accumulated score to dscore     TODO not dscore but score?
                prop_node = prop_node.parent                           # Move up in the tree
                if prop_node.dscore > self.best_so_far.dscore:
                    self.best_so_far = prop_node


            self.current = self.tree                                   # Completed backprop, set current as tree root
            self.current.fork()                                        # Fork new gamestate

    def do_action(self, action, node):
        action_str, uid, action_type, params = action
        unit = node.gs.units[uid]

        if action_type == 2:
            unit.build(*params)
        elif action_type == 3:
            unit.void()
        elif action_type == 0:
            unit.right_click(*params)
        else:
            #print(action_str, action)
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
        self.edge = action            # Which action that led to this state (Parent ---Action--- This)
        self.children = list()      # Which states derived from this state
        self.actions = list()       # Which actions can be explored from this state
        self.parent = parent        # Which state this state was derived from
        self.depth = depth          # Which depth this node are on

        self.orig_gs = None
        self.gs = gs                                                      # Fork a new gamestate
        max_p = next(p for p in self.gs.players if p.id == max_id)        # Find max player given input id
        min_p = next(p for p in self.gs.players if p != max_p)            # Find min player given max player

        self.max_p = max_p

        self.score = self.max_p.score_total()
        if self.parent:
            self.dscore = self.score - self.parent.score
        else:
            self.dscore = 0

        while True:
            self.gs.process(1, 1)
            #self.gs.clock.tick()

            if self.gs.winner:                                      # If there is a winner
                break
                #exit("Game over! %s" % self.depth)                  # Gameover!

            max_actions = MCTSAction.available_actions(max_p)       # Check for new available actions for max
            if max_actions:                                         # If no winner and no max_action
                self.actions = max_actions
                break

            min_actions = MCTSAction.available_actions(min_p)       # Check for new available actions for min
            if min_actions:
                self.actions = min_actions
                break

    def fork(self):
        self.gs = Game.load(False, self.orig_gs)