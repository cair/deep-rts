import sys
from logconf import logger
logger.info("Running PyAI using Python %s" % sys.version)

from PyAI import PyAI
from Algorithms.DQN.DQN import DQN


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
    pyai.reset()
    dqn.new_episode()
    total_cost = 0.0
    total_reward = 0.0
    frame = 0
    while not done and frame < 12000:  # 20 * 60 * 10 which is 12000   (20 minutes to seconds times 10 ticks)   # 1000  
        # Get action based on state observation
        action, values = dqn.act(observation)
        
        # Do action
        observation, reward, done, info = pyai.doAction(action)
        frame = info
        
        # Calculate cost
        total_cost += dqn.observe(reward)
        total_reward += reward

    print("Total reward", total_reward)
    print("Mean cost", total_cost/frame)
print("Done training!")