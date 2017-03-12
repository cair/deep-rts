
import sys

import time

print("Running PyAI using Python %s" % (sys.version))

import PyAPIRegistry
from PyAI import PyAI

print("Loaded PyAI")
from Algorithms.DQN.DQN import DQN
print("Loaded DQN")

pyai = PyAI(0, 1)  # Create new AI hook with gameID = 0 playerID= 0
observation_space = pyai.getState()
n_actions = 14
dqn = DQN(state_size=observation_space.shape,
              number_of_actions=n_actions,
              save_name="deeprts")  # Create new DQN using the pyai hook

num_episodes = 200000000000

PyAPIRegistry.loaded() # Tell C++ that we are done with dependencies

for e in range(num_episodes):
    observation = pyai.getState()
    done = False
    dqn.new_episode()
    total_cost = 0.0
    total_reward = 0.0
    frame = 0
    while not done:     
        frame += 1
        #env.render()
        action, values = dqn.act(observation)
        #action = env.action_space.sample()
        observation, reward, done, info = pyai.doAction(action)
        total_cost += dqn.observe(reward)
        total_reward += reward

    print("total reward", total_reward)
    print("mean cost", total_cost/frame)
print("Done training!")