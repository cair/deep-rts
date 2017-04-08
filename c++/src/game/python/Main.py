
import sys
import os
from logconf import logger
logger.info("Running PyAI using Python %s" % sys.version)
BASE_PATH = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(BASE_PATH,"Algorithms"))
for d_name in [d for d in os.listdir("Algorithms") if os.path.isdir(os.path.join(BASE_PATH,"Algorithms", d))]:
    dr = os.path.join(BASE_PATH,"Algorithms", d_name)
    if "__pycache__" in dr:
        continue

    files = os.listdir(dr)

    if "Runner.py" in files:
        logger.info("Found Runner.py which indicates a algorithm at %s. Adding..." % d_name)
        exec("from %s import %s" % (d_name, "Runner"))

    else:
        logger.info("Runner.py not found. Ignoring algorithm at %s" % d_name)














"""
# DQN Setup
"""
"""
pyai_dqn = PyAI(name="DQN")  # Create new AI instance

observation_space = pyai_dqn.getState()	# Retrieve state to shape the network
dqn = DQN(state_size=observation_space.shape,
              number_of_actions=16,
              save_name="deeprts")  # Create new DQN using the pyai hook
pyai_dqn.agent = dqn	# Set Agent to DQN
pyai_dqn.reset()
"""








"""
n_actions = 16
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

"""