
import sys
print("Running PyAI using Python %s" % (sys.version))

from Algorithms.DQN.DQN import DQN

dqn = DQN()  # Create new DQN using the pyai hook

num_episodes = 20
print("here")
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