
import PyAPIRegistry
import ctypes

from sys import getsizeof
from binascii import hexlify
import numpy as np

class PyAI:
    
    def __init__(self, name):
        self.agent = None
        self.name = name

        self.last_score = 0	# Previous recorded player score
        self.total_cost = 0
        self.total_reward = 0
        self.current_reward = 0
        self.frame = 0
        self.state_terminal = False

        self.state_array = None # byte array of current state (can be transformed etc)
        self.state_rows = None # How many rows the state has
        self.state_cols = None # How many columns the state has
        self.state_depth = None # How deep the state is (Channels)


        self.__ai__ = PyAPIRegistry.hook(self)	# Hook to the C++ object side

    def getState(self):
        np_arr = np.array(self.state_array)
        np_arr = np_arr.reshape((self.state_rows, self.state_cols, self.state_depth)).transpose()
        #np_arr = np.expand_dims(np_arr, axis=0)
        return np_arr

    def train(self, reward):
        # Calculate cost
        self.total_cost += self.agent.train(reward)
        self.total_reward += reward
        self.current_reward = reward

    def doAction(self):

        observation = self.getState()

		# Get action based on state observation
        actionID, values = self.agent.act(observation)

        return actionID

    def reset(self):
        self.state_terminal = False
        self.agent.new_episode()
        self.total_cost = 0.0
        self.total_reward = 0.0
        self.frame = 0

    def getTime(self):
        pass

    def nextFrame(self):
        pass