from PyAI import PyAI
from Algorithms.DQN.DQN import DQN

pyai_dqn = PyAI(name="DQN")  # Create new AI instance

observation_space = pyai_dqn.getState()	# Retrieve state to shape the network
dqn = DQN(state_size=observation_space.shape,
          number_of_actions=16,
          save_name="deeprts")  # Create new DQN using the pyai hook
pyai_dqn.agent = dqn	# Set Agent to DQN
pyai_dqn.reset()