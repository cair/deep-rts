import abc


class BaseAgent(abc.ABC):

	def __init__(self, env=None, player=None):
		self.env = env
		self.player = player

	@abc.abstractmethod
	def get_state(self):
		raise NotImplementedError()

	@abc.abstractmethod
	def get_action(self, obs):
		raise NotImplementedError()

	def set_player(self, player):
		self.player = player

	def set_env(self, env):
		self.env = env

	@abc.abstractmethod
	def memorize(self, obs, obs1, action, reward, terminal, info):
		raise NotImplementedError()

	@abc.abstractmethod
	def state_preprocess(self, obs):
		pass

	def save(self, filename):
		raise NotImplementedError()

	def load(self, filename):
		raise NotImplementedError()