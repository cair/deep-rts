class Agent:

	def get_action(self, state):
		raise NotImplementedError()

	def update(self, state, next_state, action, reward, terminal):
		raise NotImplementedError()

	def save(self, filename):
		raise NotImplementedError()