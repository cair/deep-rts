import numpy as np

from pyDeepRTS import PyDeepRTS

from experiment_1.agent.dqnagent import DQLearningAgent
from experiment_1.agent.random import RandomAgent


class Environment:

    def __init__(self, agents=[]):
        # Start the game
        self.g = PyDeepRTS()

        ####################################
        ##
        # Setup Players and pair with agents
        ##
        ####################################

        # Create Players
        self.players = [self.g.add_player(), self.g.add_player()]

        # Create Agent pairs
        if len(self.players) != len(agents):
            raise ValueError("Players == %s while Agent == %s. Must be equal!" % (len(self.players), len(agents)))
        self.agents = list(zip(self.players, agents))

        # Set FPS and UPS limits
        self.g.set_max_fps(10000000)
        self.g.set_max_ups(10000000)

        # How often the state should be drawn
        self.g.render_every(50)

        # How often the capture function should return a state
        self.g.capture_every(50)

        # How often the game image should be drawn to the screen
        self.g.view_every(50)

        # Start the game (flag)
        self.g.start()

    def run(self, episodes=None):
        episodes = np.iinfo(np.int32).max if episodes is None else episodes
        for episode in range(episodes):

            while not self.g.is_terminal():
                ####################################
                ##
                # Process state
                ##
                ####################################
                self.g.tick()  # Update the game clock
                self.g.update()  # Process the game state
                self.g.render()  # Draw the game state to graphics
                self.g.caption()  # Show Window caption
                self.g.view()  # View the game state in the pygame window

                ####################################
                ##
                # Perform Action
                ##
                ####################################
                state = self.g.capture()
                if state is not None:
                    for player, model in self.agents:

                        # Remember / Observe
                        model.remember(state, -0.1, False, None)

                        # Do Action
                        action = model.get_action(state)
                        player.queue_action(action, 1)

            # Reset environment
            last_state = self.g.gui.capture()
            for player, model in self.agents:
                model.remember(last_state, -1 if player.is_defeated() else 1, True, not player.is_defeated())

            self.g.reset()


if __name__ == "__main__":
    n_actions = 16
    agents = [DQLearningAgent(n_actions), RandomAgent(n_actions)]
    env = Environment(agents=agents)

    env.run()

