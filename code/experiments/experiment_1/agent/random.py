
from .base import BaseAgent
import random


class RandomAgent(BaseAgent):

    def remember(self, state, r, t, victory):
        pass

    def get_action(self, s):
        return random.randint(0, self.n_actions)

    def __init__(self, n_actions):
        super().__init__(n_actions)
