# coding:utf-8

import os
import random
import numpy as np
import tensorflow as tf
from collections import deque

from tensorflow.python.keras import Sequential
from tensorflow.python.keras.layers import Activation, TimeDistributed
from tensorflow.python.layers.convolutional import Conv2D
from tensorflow.python.layers.core import Flatten, Dense

class Agent:
    def __init__(self,
        state_dim=(84, 84),
        action_dim=16,
        frame_stack=1,
        epsilon_init=1.0,
        epsilon_final=0.1,
        epsilon_steps=1000000,

        discount=0.99,
        learning_rate=0.00025,
        batch_size=32,

        target_update_interval=10000,
        memory_init_size=200,#20000,
        memory_max_size=1000000,

        network_train_interval=50,
        network_save_interval=300000,  # Num steps before model save
        network_load=False,
        network_train=True,
        network_save_path="saved_networks/DeepRTS", network_summary_path="summary/DeepRTS"
    ):
        self.state_dim = state_dim
        self.action_dim = action_dim
        self.frame_stack = frame_stack
        self.epsilon_init = epsilon_init
        self.epsilon_final = epsilon_final
        self.epsilon_steps = epsilon_steps

        self.rms_momentum = 0.95
        self.rms_min_grad = 0.01

        self.discount = discount
        self.learning_rate = learning_rate
        self.batch_size = batch_size

        self.target_update_interval = target_update_interval
        self.memory_init_size = memory_init_size
        self.memory_max_size = memory_max_size

        self.network_train_interval = network_train_interval
        self.network_save_interval = network_save_interval  # Num steps before model save
        self.network_load = network_load
        self.network_train = network_train
        self.network_save_path = network_save_path
        self.network_summary_path = network_summary_path

        # Path creation
        os.makedirs(self.network_summary_path, exist_ok=True)
        os.makedirs(self.network_save_path, exist_ok=True)

        # Setup
        self.input_shape = (84, 84, self.frame_stack)  # (self.frame_stack, ) + self.state_dim
        self.epsilon = self.epsilon_init
        self.epsilon_decay = (self.epsilon_init - self.epsilon_final) / self.epsilon_steps
        self.t = 0

        # Parameters used for summary
        self.total_reward = 0
        self.total_q_max = 0
        self.total_loss = 0
        self.duration = 0
        self.episode = 0

        # Create replay memory
        self.replay_memory = deque(maxlen=memory_max_size)

        # Create q network
        self.s, self.q_values, q_network = self.build_network()
        q_network_weights = q_network.trainable_weights

        # Create target network
        self.st, self.target_q_values, target_network = self.build_network()
        target_network_weights = target_network.trainable_weights

        # Define target network update operation
        self.update_target_network = [
            target_network_weights[i].assign(q_network_weights[i]) for i in range(len(target_network_weights))
        ]

        # Define loss and gradient update operation
        self.a, self.y, self.loss, self.grads_update = self.build_training_op(q_network_weights)

        self.sess = tf.InteractiveSession()
        self.saver = tf.train.Saver(q_network_weights)
        self.summary_placeholders, self.update_ops, self.summary_op = self.setup_summary()
        self.summary_writer = tf.summary.FileWriter(self.network_summary_path, self.sess.graph)

        self.sess.run(tf.global_variables_initializer())

        # Load network
        if self.network_load:
            self.load_network()

        # Initialize target network
        self.sess.run(self.update_target_network)

    def build_network(self):
        model = Sequential()
        print(self.input_shape)
        model.add(Conv2D(32, (8, 8), strides=(4, 4), activation=Activation("relu"), input_shape=(84, 84, 1)))
        model.add(Conv2D(64, (4, 4), strides=(2, 2), activation=Activation("relu")))
        model.add(Conv2D(64, (3, 3), strides=(1, 1), activation=Activation("relu")))
        model.add(Flatten())
        model.add(Dense(512, activation=Activation("relu")))
        model.add(Dense(self.action_dim))

        s = tf.placeholder(tf.float32, list((None, ) + self.input_shape))
        q_values = model(s)

        return s, q_values, model

    def build_training_op(self, q_network_weights):
        a = tf.placeholder(tf.int64, [None])
        y = tf.placeholder(tf.float32, [None])

        # Convert action to one hot vector
        a_one_hot = tf.one_hot(a, self.action_dim, 1.0, 0.0)
        q_value = tf.reduce_sum(tf.multiply(self.q_values, a_one_hot), reduction_indices=1)

        # Clip the error, the loss is quadratic when the error is in (-1, 1), and linear outside of that region
        error = tf.abs(y - q_value)
        quadratic_part = tf.clip_by_value(error, 0.0, 1.0)
        linear_part = error - quadratic_part
        loss = tf.reduce_mean(0.5 * tf.square(quadratic_part) + linear_part)

        optimizer = tf.train.RMSPropOptimizer(self.learning_rate, momentum=self.rms_momentum, epsilon=self.rms_min_grad)
        grads_update = optimizer.minimize(loss, var_list=q_network_weights)

        return a, y, loss, grads_update

    def get_action(self, state):
        if self.epsilon >= random.random() or self.t < self.memory_init_size:
            action = random.randrange(self.action_dim)
        else:
            action = np.argmax(self.q_values.eval(feed_dict={self.s: [state]}))

        # Anneal epsilon linearly over time
        if self.epsilon > self.epsilon_final and self.t >= self.memory_init_size:
            self.epsilon -= self.epsilon_decay

        return action

    def run(self, state, action, reward, terminal, next_state):

        # Clip all positive rewards at 1 and all negative rewards at -1, leaving 0 rewards unchanged
        reward = np.clip(reward, -1, 1)

        # Store transition in replay memory
        self.replay_memory.append((state, action, reward, terminal, next_state))

        if self.t >= self.memory_init_size:

            # Train network
            if self.t % self.network_train_interval == 0:
                self.train_network()

            # Update target network
            if self.t % self.target_update_interval == 0:
                self.sess.run(self.update_target_network)

            # Save network
            if self.t % self.network_save_interval == 0:
                save_path = self.saver.save(self.sess, self.network_save_path + '/DeepRTS', global_step=self.t)
                print('Successfully saved: ' + save_path)


        self.total_reward += reward
        self.total_q_max += np.max(self.q_values.eval(feed_dict={self.s: [next_state]}))
        self.duration += 1

        if terminal:
            # Write summary
            if self.t >= self.memory_init_size:
                stats = [self.total_reward, self.total_q_max / float(self.duration),
                         self.duration, self.total_loss / (float(self.duration) / float(self.network_train_interval))]
                for i in range(len(stats)):
                    self.sess.run(self.update_ops[i], feed_dict={
                        self.summary_placeholders[i]: float(stats[i])
                    })
                summary_str = self.sess.run(self.summary_op)
                self.summary_writer.add_summary(summary_str, self.episode + 1)

            # Debug
            if self.t < self.memory_init_size:
                mode = 'random'
            elif self.memory_init_size <= self.t < self.memory_init_size + self.epsilon_steps:
                mode = 'explore'
            else:
                mode = 'exploit'

            print('EPISODE: {0:6d} / '
                  'TIMESTEP: {1:8d} / '
                  'DURATION: {2:5d} / '
                  'EPSILON: {3:.5f} / '
                  'TOTAL_REWARD: {4:3.0f} / '
                  'AVG_MAX_Q: {5:2.4f} / '
                  'AVG_LOSS: {6:.5f} / '
                  'MODE: {7}'.format(
                self.episode + 1, self.t, self.duration, self.epsilon,
                self.total_reward, self.total_q_max / float(self.duration),
                self.total_loss / (float(self.duration) / float(self.network_train_interval)), mode))

            self.total_reward = 0
            self.total_q_max = 0
            self.total_loss = 0
            self.duration = 0
            self.episode += 1

        self.t += 1

        return next_state

    def train_network(self):
        state_batch = []
        action_batch = []
        reward_batch = []
        next_state_batch = []
        terminal_batch = []
        y_batch = []

        # Sample random minibatch of transition from replay memory
        minibatch = random.sample(self.replay_memory, self.batch_size)
        for data in minibatch:
            state_batch.append(data[0])
            action_batch.append(data[1])
            reward_batch.append(data[2])
            next_state_batch.append(data[4])
            terminal_batch.append(data[3])

        # Convert True to 1, False to 0
        terminal_batch = np.array(terminal_batch) + 0

        next_action_batch = np.argmax(self.q_values.eval(feed_dict={self.s: next_state_batch}), axis=1)
        target_q_values_batch = self.target_q_values.eval(feed_dict={self.st: next_state_batch})
        for i in range(len(minibatch)):
            y_batch.append(reward_batch[i] + (1 - terminal_batch[i]) * self.discount * target_q_values_batch[i][next_action_batch[i]])

        loss, _ = self.sess.run([self.loss, self.grads_update], feed_dict={
            self.s: np.float32(np.array(state_batch)),
            self.a: action_batch,
            self.y: y_batch
        })

        self.total_loss += loss

    def setup_summary(self):
        episode_total_reward = tf.Variable(0.)
        tf.summary.scalar('DeepRTS/Total_Reward/Episode', episode_total_reward)
        episode_avg_max_q = tf.Variable(0.)
        tf.summary.scalar('DeepRTS/Average_Max_Q/Episode', episode_avg_max_q)
        episode_duration = tf.Variable(0.)
        tf.summary.scalar('DeepRTS/Duration/Episode', episode_duration)
        episode_avg_loss = tf.Variable(0.)
        tf.summary.scalar('DeepRTS/Average_Loss/Episode', episode_avg_loss)
        summary_vars = [episode_total_reward, episode_avg_max_q, episode_duration, episode_avg_loss]
        summary_placeholders = [tf.placeholder(tf.float32) for _ in range(len(summary_vars))]
        update_ops = [summary_vars[i].assign(summary_placeholders[i]) for i in range(len(summary_vars))]
        summary_op = tf.summary.merge_all()
        return summary_placeholders, update_ops, summary_op

    def load_network(self):
        checkpoint = tf.train.get_checkpoint_state(self.network_save_path)
        if checkpoint and checkpoint.model_checkpoint_path:
            self.saver.restore(self.sess, checkpoint.model_checkpoint_path)
            print('Successfully loaded: ' + checkpoint.model_checkpoint_path)
        else:
            print('Training new network...')

    def get_action_at_test(self, state):
        if random.random() <= 0.05:
            action = random.randrange(self.action_dim)
        else:
            action = np.argmax(self.q_values.eval(feed_dict={self.s: [state]}))

        self.t += 1

        return action

