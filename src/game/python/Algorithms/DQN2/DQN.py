
import os
import random
import numpy as np
import tensorflow as tf
from collections import deque
from skimage.color import rgb2gray
from skimage.transform import resize
from keras.models import Sequential
from keras.layers import Conv2D, Flatten, Dense
from keras import backend as K
K.set_image_dim_ordering('th')

class DQN:
	def __init__(self, 
	initial_state,
	num_actions, 
	initial_epsilon=1.0, 
	final_epsilon=0.1, 
	exploration_steps=1000000,
	initial_replay_size=10,
	memory_size=400000,
	batch_size=9,  # 32
	learning_rate=0.00025,
	momentum=0.95,
	min_grad=0.01,
	env_name="DeepRTS",
	save_network_path = "dqn2/saved_networks/",
	save_summary_path = "dqn2/summary/",
	load_network = False,
	gamma=0.99,
	train_interval = 4,
	target_update_interval = 10000,
	save_interval = 300000
	
	):
		self.state = initial_state
		self.sshape = initial_state.shape	# Shape of the state
		self.num_actions = num_actions	# Action space
		self.epsilon = initial_epsilon	# Epsilon-greedy start
		self.final_epsilon = final_epsilon	# Epsilon-greedy end
		self.epsilon_step = (self.epsilon - self.final_epsilon) / exploration_steps	# Epsilon decrease step
		self.initial_replay_size = initial_replay_size
		self.memory_size = memory_size
		self.exploration_steps = exploration_steps
		
		self.learning_rate = learning_rate
		self.momentum = momentum
		self.min_grad = min_grad
		self.batch_size = batch_size
		self.gamma = gamma
		
		self.target_update_interval = target_update_interval
		self.save_interval = save_interval
		
		self.env_name = env_name
		self.save_network_path = save_network_path + self.env_name
		self.save_summary_path = save_summary_path + self.env_name
		self.load_network = load_network
		
		
		self.train_interval = train_interval
		self.t = 0 # TODO
		
		
		# Summary Parameters
		self.total_reward = 0
		self.total_q_max = 0
		self.total_loss = 0
		self.duration = 0
		self.episode = 0
		
		# Replay Memory
		self.replay_memory = deque()
		
		# Create Q Network
		self.s, self.q_values, q_network = self.build_model()
		q_network_weights = q_network.trainable_weights
		
		# Create target network
		self.st, self.target_q_values, target_network = self.build_model()
		target_network_weights = target_network.trainable_weights
		
		# Define target network update operation
		self.update_target_network = [target_network_weights[i].assign(q_network_weights[i]) for i in range(len(target_network_weights))]

		# Define loss and gradient update operation
		self.a, self.y, self.loss, self.grads_update = self.build_functions(q_network_weights)
		
		
		self.sess = tf.InteractiveSession()
		self.saver = tf.train.Saver(q_network_weights)
		self.summary_placeholders, self.update_ops, self.summary_op = self.setup_summary()
		self.summary_writer = tf.summary.FileWriter(self.save_summary_path, self.sess.graph)

		if not os.path.exists(self.save_network_path):
			os.makedirs(self.save_network_path)

		self.sess.run(tf.global_variables_initializer())

		# Load network
		self.load()
			
		# Initialize target network
		self.sess.run(self.update_target_network)


	def build_model(self):
		model = Sequential()
		model.add(Conv2D(32, (1, 1), strides=(1, 1), activation='relu', input_shape=self.sshape))
		model.add(Conv2D(64, (1, 1), activation="relu", strides=(1, 1)))
		model.add(Conv2D(64, (1, 1), activation="relu", strides=(1, 1)))
		model.add(Flatten())
		model.add(Dense(512, activation='relu'))
		model.add(Dense(self.num_actions))

		s = tf.placeholder(tf.float32, [None, *self.sshape])
		q_values = model(s)

		return s, q_values, model
	
	def build_functions(self, q_network_weights):
		a = tf.placeholder(tf.int64, [None])
		y = tf.placeholder(tf.float32, [None])

		# Convert action to one hot vector
		a_one_hot = tf.one_hot(a, self.num_actions, 1.0, 0.0)
		q_value = tf.reduce_sum(tf.multiply(self.q_values, a_one_hot), reduction_indices=1)

		# Clip the error, the loss is quadratic when the error is in (-1, 1), and linear outside of that region
		error = tf.abs(y - q_value)
		quadratic_part = tf.clip_by_value(error, 0.0, 1.0)
		linear_part = error - quadratic_part
		loss = tf.reduce_mean(0.5 * tf.square(quadratic_part) + linear_part)

		optimizer = tf.train.RMSPropOptimizer(self.learning_rate, momentum=self.momentum, epsilon=self.min_grad)
		grads_update = optimizer.minimize(loss, var_list=q_network_weights)

		return a, y, loss, grads_update

		
	def new_episode(self):
		pass
	

	def end_episode(self):
		pass

	def act(self):
		if self.epsilon >= random.random() or self.t < self.initial_replay_size:
			action = random.randrange(self.num_actions)
		else:
			action = np.argmax(self.q_values.eval(feed_dict={self.s: [np.float32(self.state)]}))

		# Anneal epsilon linearly over time
		if self.epsilon > self.final_epsilon and self.t >= self.initial_replay_size:
			self.epsilon -= self.epsilon_step

		return action

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
			next_state_batch.append(data[3])
			terminal_batch.append(data[4])

		# Convert True to 1, False to 0
		terminal_batch = np.array(terminal_batch) + 0

		
		target_q_values_batch = self.target_q_values.eval(feed_dict={self.st: np.float32(np.array(next_state_batch))})
		y_batch = reward_batch + (1 - terminal_batch) * self.gamma * np.max(target_q_values_batch, axis=1)

		loss, _ = self.sess.run([self.loss, self.grads_update], feed_dict={
			self.s: np.float32(np.array(state_batch)),
			self.a: action_batch,
			self.y: y_batch
		})

		self.total_loss += loss


	def train(self, action, reward, terminal, observation):
		"""
		# action - The performed action which led to this state
		# reward - The reward given in the state transition
		# terminal - Is state terminal? (Loss / Victory)
		# observation - New state observation after action
		"""
		
		next_state = np.append(self.state[1:, :, :], observation, axis=0)
	
		# Clip all positive rewards at 1 and all negative rewards at -1, leaving 0 rewards unchanged
		reward = np.clip(reward, -1, 1)
		
		# Store transition in replay memory
		self.replay_memory.append((self.state, action, reward, self.state, terminal))
		if len(self.replay_memory) > self.memory_size:
			self.replay_memory.popleft()
			
			
		if self.t >= self.initial_replay_size:
			# Train network
			if self.t % self.train_interval == 0:
				self.train_network()

			# Update target network
			if self.t % self.target_update_interval == 0:
				self.sess.run(self.update_target_network)

			# Save network
			if self.t % self.save_interval == 0:
				save_path = self.saver.save(self.sess, self.save_network_path + '/' + self.env_name, global_step=self.t)
				print('Successfully saved: ' + save_path)

				
		self.total_reward += reward
		self.total_q_max += np.max(self.q_values.eval(feed_dict={self.s: [np.float32(self.state)]}))
		self.duration += 1
		
		
		if terminal:
			# Write summary
			if self.t >= self.initial_replay_size:
				stats = [self.total_reward, self.total_q_max / float(self.duration),self.duration, self.total_loss / (float(self.duration) / float(self.train_interval))]
			
			for i in range(len(stats)):
				self.sess.run(self.update_ops[i], feed_dict={self.summary_placeholders[i]: float(stats[i])})
				
			summary_str = self.sess.run(self.summary_op)
			self.summary_writer.add_summary(summary_str, self.episode + 1)


			# Debug
			if self.t < self.initial_replay_size:
				mode = 'random'
			elif self.initial_replay_size <= self.t < self.initial_replay_size + self.exploration_steps:
				mode = 'explore'
			else:
				mode = 'exploit'
			print('EPISODE: {0:6d} / TIMESTEP: {1:8d} / DURATION: {2:5d} / EPSILON: {3:.5f} / TOTAL_REWARD: {4:3.0f} / AVG_MAX_Q: {5:2.4f} / AVG_LOSS: {6:.5f} / MODE: {7}'.format(self.episode + 1, self.t, self.duration, self.epsilon,self.total_reward, self.total_q_max / float(self.duration),self.total_loss / (float(self.duration) / float(self.train_interval)), mode))

			self.total_reward = 0
			self.total_q_max = 0
			self.total_loss = 0
			self.duration = 0
			self.episode += 1

		self.t += 1
				
			

	def iterate(self):
		pass
		
	def load(self):
		checkpoint = tf.train.get_checkpoint_state(self.save_network_path)
		if self.load_network and checkpoint and checkpoint.model_checkpoint_path:
			self.saver.restore(self.sess, checkpoint.model_checkpoint_path)
			print('Successfully loaded: ' + checkpoint.model_checkpoint_path)
		else:
			print('Training new network...')
		
	def setup_summary(self):
		episode_total_reward = tf.Variable(0.)
		tf.summary.scalar(self.env_name + '/Total Reward/Episode', episode_total_reward)
		episode_avg_max_q = tf.Variable(0.)
		tf.summary.scalar(self.env_name + '/Average Max Q/Episode', episode_avg_max_q)
		episode_duration = tf.Variable(0.)
		tf.summary.scalar(self.env_name + '/Duration/Episode', episode_duration)
		episode_avg_loss = tf.Variable(0.)
		tf.summary.scalar(self.env_name + '/Average Loss/Episode', episode_avg_loss)
		summary_vars = [episode_total_reward, episode_avg_max_q, episode_duration, episode_avg_loss]
		summary_placeholders = [tf.placeholder(tf.float32) for _ in range(len(summary_vars))]
		update_ops = [summary_vars[i].assign(summary_placeholders[i]) for i in range(len(summary_vars))]
		summary_op = tf.summary.merge_all()
		return summary_placeholders, update_ops, summary_op