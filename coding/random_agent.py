from agent import Agent
from DeepRTS.Engine import Random

class RandomAgent(Agent):

	def get_action(self, state):
		return Random.action() - 1

	def update(self, state, next_state, action, reward, terminal):
		return

	def save(self, file_name):
		return