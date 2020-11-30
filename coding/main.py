
# Run setup.py install in sources/python to build source files.
from DeepRTS.python import scenario
from DeepRTS.Engine import Random
from DeepRTS.python.scenario.engine import Scenario
from DeepRTS.python import util, Config, Game
from DeepRTS import Engine

import time

import pickle

import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import numpy as np
import pandas as pd
import random
from collections import deque, defaultdict, namedtuple

if __name__ == "__main__":
    episodes = 10
    random_play = True

    result = open("result.txt", "w")
    result.write("Results from experiment:\n")

    env = scenario.Scenario182({})

    env.game.set_max_fps(99999999)
    env.game.set_max_ups(99999999)

    scores_a = []
    scores_b = []

    durations = []

    class QNetwork(nn.Module):
        def __init__(self, state_size, action_size, seed):
            """
            Build a fully connected neural network

            Parameters
            ----------
            state_size (int): State dimension
            action_size (int): Action dimension
            seed (int): random seed
            """
            super(QNetwork, self).__init__()
            self.seed = torch.manual_seed(seed)
            self.fc1 = nn.Linear(state_size, 32)
            self.fc2 = nn.Linear(32, 64)
            self.fc3 = nn.Linear(64, action_size)

        def forward(self, x):
            """Forward pass"""
            x = F.relu(self.fc1(x))
            x = F.relu(self.fc2(x))
            x = self.fc3(x)

            return x
    class ReplayBuffer:
        def __init__(self, buffer_size, batch_size, seed):
            """
            Replay memory allow agent to record experiences and learn from them

            Parametes
            ---------
            buffer_size (int): maximum size of internal memory
            batch_size (int): sample size from experience
            seed (int): random seed
            """
            self.batch_size = batch_size
            self.seed =0
            self.memory = deque(maxlen=buffer_size)
            self.experience = namedtuple("Experience", field_names=["state", "action", "reward", "next_state", "done"])

        def add(self, state, action, reward, next_state, done):
            """Add experience"""
            experience = self.experience(state, action, reward, next_state, done)
            self.memory.append(experience)

        def sample(self):
            """
            Sample randomly and return (state, action, reward, next_state, done) tuple as torch tensors
            """
            experiences = random.sample(self.memory, k=self.batch_size)

            # Convert to torch tensors
            states = torch.from_numpy(np.vstack([experience.state for experience in experiences if experience is not None])).float()
            actions = torch.from_numpy(np.vstack([experience.action for experience in experiences if experience is not None])).long()
            rewards = torch.from_numpy(np.vstack([experience.reward for experience in experiences if experience is not None])).float()
            next_states = torch.from_numpy(np.vstack([experience.next_state for experience in experiences if experience is not None])).float()
            # Convert done from boolean to int
            dones = torch.from_numpy(np.vstack([experience.done for experience in experiences if experience is not None]).astype(np.uint8)).float()

            return (states, actions, rewards, next_states, dones)

        def __len__(self):
            return len(self.memory)
    class DQNAgent:
        def __init__(self, state_size, action_size, seed):
            """
            DQN Agent interacts with the environment,
            stores the experience and learns from it

            Parameters
            ----------
            state_size (int): Dimension of state
            action_size (int): Dimension of action
            seed (int): random seed
            """
            self.state_size = state_size
            self.action_size = action_size
            self.seed = random.seed(seed)
            # Initialize Q and Fixed Q networks
            self.q_network = QNetwork(state_size, action_size, seed)
            self.fixed_network = QNetwork(state_size, action_size, seed)
            self.optimizer = optim.Adam(self.q_network.parameters())
            # Initiliase memory
            self.memory = ReplayBuffer(BUFFER_SIZE, BATCH_SIZE, seed)
            self.timestep = 0


        def step(self, state, action, reward, next_state, done):
            """
            Update Agent's knowledge

            Parameters
            ----------
            state (array_like): Current state of environment
            action (int): Action taken in current state
            reward (float): Reward received after taking action
            next_state (array_like): Next state returned by the environment after taking action
            done (bool): whether the episode ended after taking action
            """
            self.memory.add(state, action, reward, next_state, done)
            self.timestep += 1
            if self.timestep % UPDATE_EVERY == 0:
                if len(self.memory) > BATCH_SIZE:
                    sampled_experiences = self.memory.sample()
                    self.learn(sampled_experiences)

        def learn(self, experiences):
            """
            Learn from experience by training the q_network

            Parameters
            ----------
            experiences (array_like): List of experiences sampled from agent's memory
            """
            states, actions, rewards, next_states, dones = experiences
            # Get the action with max Q value
            action_values = self.fixed_network(next_states).detach()
            # Notes
            # tensor.max(1)[0] returns the values, tensor.max(1)[1] will return indices
            # unsqueeze operation --> np.reshape
            # Here, we make it from torch.Size([64]) -> torch.Size([64, 1])
            max_action_values = action_values.max(1)[0].unsqueeze(1)

            # If done just use reward, else update Q_target with discounted action values
            Q_target = rewards + (GAMMA * max_action_values * (1 - dones))
            Q_expected = self.q_network(states).gather(1, actions)

            # Calculate loss
            loss = F.mse_loss(Q_expected, Q_target)
            self.optimizer.zero_grad()
            # backward pass
            loss.backward()
            # update weights
            self.optimizer.step()

            # Update fixed weights
            self.update_fixed_network(self.q_network, self.fixed_network)

        def update_fixed_network(self, q_network, fixed_network):
            """
            Update fixed network by copying weights from Q network using TAU param

            Parameters
            ----------
            q_network (PyTorch model): Q network
            fixed_network (PyTorch model): Fixed target network
            """
            for source_parameters, target_parameters in zip(q_network.parameters(), fixed_network.parameters()):
                target_parameters.data.copy_(TAU * source_parameters.data + (1.0 - TAU) * target_parameters.data)


        def act(self, state, eps=0):
            """
            Choose the action

            Parameters
            ----------
            state (array_like): current state of environment
            eps (float): epsilon for epsilon-greedy action selection
            """
            rnd = random.random()
            if rnd < eps:
                return np.random.randint(self.action_size)
            else:
                state = torch.from_numpy(state).float().unsqueeze(0)
                # set the network into evaluation mode
                self.q_network.eval()
                with torch.no_grad():
                    action_values = self.q_network(state)
                # Back to training mode
                self.q_network.train()
                action = np.argmax(action_values.cpu().data.numpy())
                return action

        def checkpoint(self, filename):
            torch.save(self.q_network.state_dict(), filename)

    # env = scenario.GoldThousand(game)
    BUFFER_SIZE = int(1e5) # Replay memory size
    BATCH_SIZE = 64         # Number of experiences to sample from memory
    GAMMA = 0.99            # Discount factor
    TAU = 1e-3              # Soft update parameter for updating fixed q network
    LR = 1e-4               # Q Network learning rate
    UPDATE_EVERY = 4        # How often to update Q network

    state_size = env.observation_space.shape[0]
    action_size = env.action_space.n

    dqn_agent = DQNAgent(state_size, action_size, seed=0)

    torch.save(dqn_agent.q_network.state_dict(), "/Users/diegogutierrez/Documents/college/semester_3/COMPSCI_182/final_project/myfork/before.pt")

    for episode in range(episodes):
        print("Episode: %s, FPS: %s, UPS: %s" % (episode, env.game.get_fps(), env.game.get_ups()), flush = True)

        terminal = False
        state = env.reset()
        score_a = 0
        score_b = 0

        while not terminal:

            # AI for player 1
            env.game.set_player(env.game.players[0])

            # chooses a random action
            action = Random.action() - 1
            next_state, reward, terminal, _ = env.step(action)

            score_a += reward




            # AI for player 2
            env.game.set_player(env.game.players[1])

            # chooses an action based on a neural network
            action = dqn_agent.act(state, 0.2)
            next_state, reward, done, info = env.step(action)
            dqn_agent.step(state, action, reward, next_state, done)
            state = next_state

            score_b += reward

        scores_a.append(score_a)
        scores_b.append(score_b)

        durations.append(env.game.get_episode_duration())


    torch.save(dqn_agent.q_network.state_dict(), "/Users/diegogutierrez/Documents/college/semester_3/COMPSCI_182/final_project/myfork/after.pt")
    # pickle.dump(dqn_agent, open("model", "wb"))

    for score in scores_a:

        result.write(str(score) + ",")

    result.write("\n")

    for score in scores_b:

        result.write(str(score) + ",")

    result.write("\n")

    for duration in durations:

        result.write(str(duration) + ",")

    result.write("\n")






