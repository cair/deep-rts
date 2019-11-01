from .q_learning.Agent import Agent
from .q_learning.Statistics import Statistics
from experiment_1.agent.base import BaseAgent

from skimage.transform import resize
from skimage.color import rgb2gray
import numpy as np


class DQLearningAgent(BaseAgent):

    def __init__(self, n_actions):
        super().__init__(n_actions)
        self.stats = Statistics()
        self.agent = Agent()

        self.last_terminal = None
        self.last_reward = None
        self.last_action = None
        self.s0 = None
        self.s1 = None

    def preprocess(self, s):
        s = resize(s, (84, 84))
        s = rgb2gray(s)
        #s = np.reshape(s, (1, ) + s.shape)
        s = np.reshape(s, s.shape + (1, ))
        return s

    def remember(self, s_now, r, t):
        self.s0 = self.s1
        self.s1 = s_now
        a = self.last_action

        if self.s0 is not None and self.s1 is not None and a is not None:
            s0 = self.preprocess(self.s0)
            s1 = self.preprocess(self.s1)
            self.agent.run(s0, a, r, t, s1)

    def get_action(self, s):
        action = self.agent.get_action(self.preprocess(s))
        self.last_action = action
        return action
