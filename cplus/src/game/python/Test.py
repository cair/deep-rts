from PyAI import PyAI
import numpy as np
from cplus.src.game.python.Algorithms.DQN.DQN import DQN

pyai_dqn = PyAI(name="DQN")  # Create new AI instance

observation_space = pyai_dqn.getState()	 # Retrieve state to shape the network


dqn = DQN(state_size=observation_space.shape,
          number_of_actions=16,
          save_name="deeprts")  # Create new DQN using the pyai hook

pyai_dqn.agent = dqn	# Set Agent to DQN
pyai_dqn.reset()


pyai_dqn.train(1)
pyai_dqn.doAction()