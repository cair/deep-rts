
import sys
from PyAI import PyAI
from Algorithms.DQN.DQN import DQN
print("Running PyAI using Python %s" % (sys.version))
PyAPIRegistry.loaded() # Tell C++ that we are done with dependencies

pyai = PyAI(0, 0)  # Create new AI hook with gameID = 0 playerID= 0
dqn = DQN()  # Create new DQN using the pyai hook

num_episodes = 20

for e in xrange(num_episodes):
    observation = pyai.getState()
    done = False
    agent.new_episode()
    total_cost = 0.0
    total_reward = 0.0
    frame = 0
    while not done:
        frame += 1
        #env.render()
        action, values = dqn.act(observation)
        #action = env.action_space.sample()
        observation, reward, done, info = env.step(action)
        total_cost += agent.observe(reward)
        total_reward += reward
    print("total reward", total_reward)
    print("mean cost", total_cost/frame)