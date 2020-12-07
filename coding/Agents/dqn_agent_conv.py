import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torch.autograd as autograd
import numpy as np
import pandas as pd
import random
from collections import deque, defaultdict, namedtuple

from Agents import Agent

class ConvDQN(nn.Module):

    def __init__(self, input_shape, output_dim):
        super(ConvDQN, self).__init__()
        self.input_shape = input_shape
        self.output_dim = output_dim

        self.conv = nn.Sequential(
            nn.Conv2d(self.input_shape[0], 9*9*64, kernel_size=8, stride=4),
            nn.ReLU(),
            nn.Conv2d(9*9*64, 7*7*64, kernel_size=4, stride=2),
            nn.ReLU(),
            nn.Conv2d(7*7*64, 512, kernel_size=3, stride=1),
            nn.ReLU()
        )

        self.fc = nn.Sequential(
            nn.Linear(512, 512),
            nn.ReLU(),
            nn.Linear(512, 512),
            nn.ReLU(),
            nn.Linear(512, self.output_dim)
        )
        self.device = T.device('cuda:0' if T.cuda.is_available() else 'cpu')
        self.to(self.device)

    def forward(self, state):
        features = self.conv(state)
        qvals = self.fc(features)
        return qvals

class BasicBuffer:

    def __init__(self, max_size):
        self.max_size = max_size
        self.buffer = deque(maxlen=max_size)

    def push(self, state, action, reward, next_state, done):
        experience = (state, action, np.array([reward]), next_state, done)
        self.buffer.append(experience)

    def sample(self, batch_size):
        state_batch = []
        action_batch = []
        reward_batch = []
        next_state_batch = []
        done_batch = []

        batch = random.sample(self.buffer, batch_size)

        for experience in batch:
            state, action, reward, next_state, done = experience
            state_batch.append(state)
            action_batch.append(action)
            reward_batch.append(reward)
            next_state_batch.append(next_state)
            done_batch.append(done)

        return (state_batch, action_batch, reward_batch, next_state_batch, done_batch)

    def __len__(self):
        return len(self.buffer)

class ConvAgent:

    def __init__(self, obs_space, act_space, use_conv=True, learning_rate=3e-4, gamma=0.99, buffer_size=10000):
        self.learning_rate = learning_rate
        self.gamma = gamma
        self.replay_buffer = BasicBuffer(max_size=buffer_size)
        self.model = ConvDQN(obs_space, act_space)
        self.optimizer = torch.optim.Adam(self.model.parameters())
        self.MSE_loss = nn.MSELoss()

    def get_action(self, state):
        state  = torch.from_numpy(state).float()
        # state = state.unsqueeze(1)  # 21
        # state = state.unsqueeze(1)  # 21
        # state = state.unsqueeze(0)
        # state = state.numpy()
        # print(state)
        # state[2] = 21
        # state[3] = 21
        # state = torch.from_numpy(state).float()
        # state = autograd.Variable(torch.from_numpy(state).float().unsqueeze(0))
        qvals = self.model.forward(state)
        action = np.argmax(qvals.detach().numpy())

        return action

    def compute_loss(self, batch):
        states, actions, rewards, next_states, dones = batch
        states = torch.FloatTensor(states)
        actions = torch.LongTensor(actions)
        rewards = torch.FloatTensor(rewards)
        next_states = torch.FloatTensor(next_states)
        dones = torch.FloatTensor(dones)

        curr_Q = self.model.forward(states).gather(1, actions.unsqueeze(1))
        curr_Q = curr_Q.squeeze(1)
        next_Q = self.model.forward(next_states)
        max_next_Q = torch.max(next_Q, 1)[0]
        expected_Q = rewards.squeeze(1) + (1 - dones) * self.gamma * max_next_Q

        loss = self.MSE_loss(curr_Q, expected_Q.detach())
        return loss

    def update(self, batch_size):
        batch = self.replay_buffer.sample(batch_size)
        loss = self.compute_loss(batch)

        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()
