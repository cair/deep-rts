import random
import numpy as np
import os

from tensorflow import Session, ConfigProto
from tensorflow.python.keras import backend as K
from tensorflow.python.keras._impl.keras.optimizers import RMSprop
from tensorflow.python.keras.callbacks import CSVLogger
from tensorflow.python.keras.optimizers import Adam

from experiment_1.agent.dqn.loss_functions import huber_loss_1
from .dqn.statistics import Statistics
from .base import BaseAgent
from .dqn.models import cnn
from .dqn.callbacks import TargetModelUpdateCallback, ModelIntervalCheckpoint, LossCallback

K.set_session(Session(config=ConfigProto(inter_op_parallelism_threads=2)))

dir_path = os.path.dirname(os.path.realpath(__file__))

from skimage.transform import resize
from skimage.color import rgb2gray
import numpy as np


class Memory:

    def __init__(self, memory_size):
        self.buffer = []
        self.count = 0
        self.max_memory_size = memory_size

    def _recalibrate(self):
        self.count = len(self.buffer)

    def remove_n(self, n):
        self.buffer = self.buffer[n - 1:-1]
        self._recalibrate()

    def add(self, memory):
        self.buffer.append(memory)
        self.count += 1

        if self.count > self.max_memory_size:
            self.buffer.pop(0)
            self.count -= 1

    def get(self, batch_size=1):
        if self.count <= batch_size:
            return np.array(self.buffer)

        return np.array(random.sample(self.buffer, batch_size))


class DQLearningAgent(BaseAgent):

    def __init__(self, n_actions):
        super().__init__(n_actions)
        self.action_space = [1, 2, 3, 4, 5, 12, 13]
        n_actions = len(self.action_space)
        self.n_actions = n_actions
        self.action_names = [
            "PrvU",
            "NxtU",
            "MoLe",
            "MoRi",
            "MoUp",
            "MoDo",
            "MoUL",
            "MoUR",
            "MoDL",
            "MoDR",
            "A",
            "H",
            "B0",
            "B1",
            "B2",
            "NOP"
        ]

        self.agent = Agent((84, 84, 1), n_actions, cnn)
        self.statistics = Statistics(len(self.action_names), self.action_names)

        self.last_terminal = None
        self.last_reward = None
        self.last_action = None
        self.s0 = None
        self.s1 = None
        self.count = 0
        self.victories = 0
        self.episode = 1

    def preprocess(self, s):
        s = resize(s, (84, 84))
        s = rgb2gray(s)
        s = np.reshape(s, (1,) + s.shape)
        s = np.reshape(s, s.shape + (1,))
        return s

    def remember(self, s_now, r, t, victory):
        self.s0 = self.s1
        self.s1 = s_now
        a = self.last_action

        if self.s0 is not None and self.s1 is not None and a is not None:
            s0 = self.preprocess(self.s0)
            s1 = self.preprocess(self.s1)

            #if np.array_equal(s0, s1):
            #    print("Equal with action: " + self.action_names[self.action_space[a]])

            self.agent.memory.add((s0, a, r, s1, t))

        if t:
            self.victories += victory
            self.statistics.add_win_percent(self.victories / self.episode)
            self.episode += 1
            self.statistics.next_episode(self.episode)
            self.statistics.plot()

        if self.count % 100 == 0:
            self.agent.train()
            self.statistics.add_loss(self.agent.epoch, self.agent.loss)

        self.count += 1

    def get_action(self, s):
        orig_action = self.agent.act(self.preprocess(s))
        translated_action = self.action_space[orig_action]

        self.statistics.add_action(translated_action)
        self.last_action = orig_action
        return translated_action + 1


class Agent:
    def __init__(self,
                 observation_space,
                 action_space,
                 model,
                 lr=1e-4,
                 exploration_episodes=50,
                 memory_size=1000000,
                 e_start=1.0,
                 e_end=0.0,
                 e_steps=10000,
                 batch_size=64,
                 discount=0.99,
                 load_checkpoint=True,
                 use_double=True,
                 ):

        # File definitions
        self.checkpoint_file = os.path.join(dir_path, "checkpoint.hdf5")
        self.logger_file = os.path.join(dir_path, "log.csv")

        self.use_double = use_double

        self.observation_space = observation_space
        self.action_space = action_space

        self.memory = Memory(memory_size)

        # Hyperparameters
        self.LEARNING_RATE = lr
        self.BATCH_SIZE = batch_size
        self.GAMMA = discount

        # Epsilon decent
        self.EPSILON_START = e_start
        self.EPSILON_END = e_end
        self.EPSILON_DECAY = (self.EPSILON_END - self.EPSILON_START) / e_steps
        self.epsilon = self.EPSILON_START

        self.loss = None
        self.epoch = 0

        self.model_callbacks = []
        self.model = model(self.observation_space, self.action_space)
        self.target_model = model(self.observation_space, self.action_space) if self.use_double else None

        if load_checkpoint:
            try:
                self.model.load_weights(self.checkpoint_file)
            except OSError:
                pass

            try:
                self.target_model.load_weights(self.checkpoint_file)
            except OSError:
                pass

        # Compile models
        optimizer = RMSprop(lr=self.LEARNING_RATE)
        loss = huber_loss_1  # "mse"
        metrics = ["accuracy"]
        self.model.compile(optimizer=optimizer, loss=loss, metrics=metrics)
        if self.use_double:
            target_updater = TargetModelUpdateCallback(self.target_model, 5, verbose=1)
            self.target_model.compile(optimizer=optimizer, loss=loss, metrics=metrics)
            self.model_callbacks.append(target_updater)

        # Callbacks
        self.model_callbacks.append(ModelIntervalCheckpoint(self.checkpoint_file, interval=50, verbose=1))
        self.model_callbacks.append(CSVLogger(self.logger_file, separator=',', append=True))
        self.model_callbacks.append(LossCallback(interval=5))

        self.episode = 1
        self.exploration_episodes = exploration_episodes

        print("State size is: %s,%s,%s" % self.observation_space)
        print("Action size is: %s" % self.action_space)
        print("Batch size is: %s " % self.BATCH_SIZE)

    def reset(self):
        pass

    def next_episode(self):
        self.episode += 1

    def update_target_model(self):
        # copy weights from model to target_model
        self.target_model.set_weights(self.model.get_weights())

    def train(self):
        if self.memory.count < self.BATCH_SIZE:
            return

        # Define which models to do updates on
        m1 = self.model
        m2 = self.target_model if self.use_double else self.model

        # Define inputs to model, and which targets (outputs) to predict
        inputs = np.zeros(((self.BATCH_SIZE,) + self.observation_space))
        targets = np.zeros((self.BATCH_SIZE, self.action_space))

        cnt = 0
        for i, (s, a, r, s1, terminal) in enumerate(self.memory.get(self.BATCH_SIZE)):
            target = r

            #if np.array_equal(s, s1):
            #    cnt += 1

            if not terminal:
                tar_s1 = m2.predict(s1)
                target = r + self.GAMMA * np.amax(tar_s1[0])

            targets[i] = m2.predict(s)
            targets[i, a] = target
            inputs[i] = s

        #print(cnt / self.BATCH_SIZE)

        history = m1.fit(inputs, targets, epochs=1, callbacks=self.model_callbacks, verbose=0)
        self.loss = history.history["loss"][0]
        self.epoch += 1

        # if self.ddqn and self.episode_train_count % 50 == 0:
        #    self.update_target_model()

    def act(self, state):
        self.epsilon = max(self.EPSILON_END, self.epsilon + self.EPSILON_DECAY)

        # Epsilon exploration
        if np.random.uniform() <= self.epsilon or self.episode < self.exploration_episodes:
            return random.randrange(self.action_space)

        # Exploit Q-Knowledge
        act_values = self.target_model.predict(state) if self.use_double else self.model.predict(state)

        return np.argmax(act_values[0])


if __name__ == "__main__":
    agent = Agent((84, 84, 1), 4, cnn)

    for x in range(1000):
        agent.memory.add((
            np.zeros((1, 84, 84, 1)),
            random.randint(0, 3),
            random.randrange(-1, 1),
            np.zeros((1, 84, 84, 1)),
            False
        ))

    for x in range(10000):
        agent.train()
