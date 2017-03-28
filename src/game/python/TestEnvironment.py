from Algorithms.DQN2.DQN import DQN
import numpy as np
import random
def getState():

	state = np.ndarray(shape=(6,30,30), dtype=float)
	return state
	
def reward(action):
	if (action == 2):
		return 1
	return 0
	
	
	
observation = getState()
actions = [0, 1, 2, 3]

dqn = DQN(observation, len(actions))
	
# This emulates C++
i = 0
while True:
	
	# Get Action from network
	actionID = dqn.act()
	
	# Do action in game
	# observation, reward, terminal = game.doAction(actionID)
	# SOME TIME LATER
	####
	
	# Get state after action
	observation = getState()	# Observation
	terminal = True if i > 100 else False
	reward_ = reward(actionID)

	dqn.train(actionID, reward_, terminal, observation)
	
	print(actionID, reward_)
	i += 1
	


	

