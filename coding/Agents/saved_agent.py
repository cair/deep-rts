import torch
from Agents import Agent

class SavedAgent(Agent):

	def __init__(self, path, state_size, action_size, seed):

		this.network = QNetwork(state_size, action_size, seed)
		this.network.load_state_dict(torch.load(path))
		this.network.eval()

	def get_action(self, state):

        with torch.no_grad():
            action_values = self.q_network(state)

		action = np.argmax(action_values.cpu().data.numpy())
		return action

	def update(self, state, next_state, action, reward, terminal):

		return

	def save(self, filename)

		return

