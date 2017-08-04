import os
import random
import numpy as np
import keras
from keras import backend as K
from keras.utils import plot_model
from tensorflow.contrib.keras.python.keras import initializers
from tensorflow.contrib.keras.python.keras.optimizers import rmsprop, adam
from tensorflow.contrib.keras.python.keras.engine import Input, Model
from tensorflow.contrib.keras.python.keras.layers.convolutional import Conv2D
from tensorflow.contrib.keras.python.keras.layers.core import Activation, Flatten, Dense


class DQN:
    def __init__(self, state_size, number_of_actions, save_name):
        self.memory = []

        # Parameters
        self.LEARNING_RATE = 0.0001
        self.BATCH_SIZE = 32
        self.GAMMA = 0.9

        # Epsilon decent
        self.EPSILON_START = 1
        self.EPSILON_END = 0.1
        self.EPSILON_DECAY = (self.EPSILON_END - self.EPSILON_START) / 1000000
        self.epsilon = self.EPSILON_START

        # Episode data
        self.episode = 0  # Episode Count
        self.episode_loss = 0  # Loss sum of a episode
        self.episode_reward = 0  # Reward sum of a episode
        self.iteration = 0  # Iteration counter

        # State data
        self.state_size = state_size
        self.action_size = number_of_actions
        self.target_model = self.build_model()
        self.model = self.build_model()

    def build_model(self):
        # Neural Net for Deep-Q learning Model
        initializer = initializers.random_normal(stddev=0.01)

        # Image input
        input_layer = Input(shape=self.state_size, name='image_input')
        conv1 = Conv2D(32, (2, 2), strides=(4, 4), activation='relu', kernel_initializer=initializer)(input_layer)
        conv2 = Conv2D(64, (1, 1), strides=(2, 2), activation='relu', kernel_initializer=initializer)(conv1)
        conv3 = Conv2D(64, (1, 1), strides=(1, 1), activation='relu', kernel_initializer=initializer)(conv2)
        conv_flatten = Flatten()(conv3)

        # Stream split
        fc1 = Dense(512, kernel_initializer=initializer, activation="relu")(conv_flatten)
        fc2 = Dense(512, kernel_initializer=initializer, activation="relu")(conv_flatten)

        advantage = Dense(self.action_size)(fc1)
        value = Dense(1)(fc2)

        # policy = keras.layers.merge([advantage, value], mode=lambda x: x[0]-K.mean(x[0])+x[1],  output_shape=(self.action_size,))
        policy = Dense(self.action_size, activation="linear")(fc1)

        model = Model(inputs=[input_layer], outputs=[policy])
        # model = Model(inputs=[input_layer, input_layer_2], outputs=[policy])
        optimizer = adam(self.LEARNING_RATE)
        model.compile(optimizer=optimizer, loss="mse")
        plot_model(model, to_file='./model.png', show_shapes=True, show_layer_names=True)

        return model

    def reset(self):
        pass

    def new_episode(self):
        self.iteration = 0

        # Reset loss sum
        self.episode_loss = 0

        # Reset episode reward
        self.episode_reward = 0

        # Increase episode
        self.episode += 1

    def end_episode(self):
        pass

    def act(self, state):
        if np.random.rand() <= self.epsilon:
            return random.randrange(self.action_size), 0

        # Exploit Q-Knowledge
        act_values = self.target_model.predict(state)
        return np.argmax(act_values[0]), 0  # returns action

    def train(self, reward):
        if len(self.memory) < self.BATCH_SIZE:
            return 0

        loss = 0
        memories = random.sample(self.memory, self.BATCH_SIZE)
        for (s, a, r, s1, terminal) in memories:
            target = self.model.predict(s)

            if terminal:
                target[0, a] = r
            else:
                pred_a = self.model.predict(s)
                pred_t = self.target_model.predict(s1)[0]

                target[0, a] = r + self.GAMMA * pred_t[np.argmax(pred_a)]

            history = self.model.fit(s, target, epochs=1, batch_size=1, callbacks=[], verbose=0)
            loss += history.history["loss"][0]

        self.episode_loss += loss

        return loss
