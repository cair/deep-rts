import math
import random
import numpy
from .model import Brain
from .memory import Memory


class Agent:

    MEMORY_CAPACITY = 200000
    BATCH_SIZE = 32
    GAMMA = 0.99
    MAX_EPSILON = 1
    MIN_EPSILON = 0.1
    EXPLORATION_STOP = 500000  # at this step epsilon will be 0.01
    LAMBDA = - math.log(0.01) / EXPLORATION_STOP  # speed of decay
    UPDATE_TARGET_FREQUENCY = 50

    epsilon = MAX_EPSILON

    def __init__(self, state_shape, action_count):
        self.state_shape = state_shape
        self.action_count = action_count

        self.brain = Brain(state_shape, action_count)
        self.memory = Memory(Agent.MEMORY_CAPACITY)
        self.steps = 0
        self.generation = 0

    def act(self, s):
        if random.random() < self.epsilon:
            return random.randint(0, self.action_count - 1)
        else:
            return numpy.argmax(self.brain.predict_one(s))

    def observe_no_update(self, sample):
        x, y, errors = self._get_targets([(0, sample)])
        return x, y, errors


    def observe(self, sample):  # in (s0, a, r, s1) format
        x, y, errors = self._get_targets([(0, sample)])

        self.memory.add(errors[0], sample)

        if self.steps % Agent.UPDATE_TARGET_FREQUENCY == 0:
            self.brain.update_target_model()

        # slowly decrease Epsilon based on our eperience
        self.steps += 1
        self.epsilon = Agent.MIN_EPSILON + (Agent.MAX_EPSILON - Agent.MIN_EPSILON) * math.exp(-Agent.LAMBDA * self.steps)

    def get_steps(self):
        return self.steps

    def _get_targets(self, batch):
        # Terminology:
        # o: observation
        # s: state
        # a: action
        # r: reward
        # s_ state + 1

        no_state = numpy.zeros(self.state_shape)

        states = numpy.array([o[1][0] for o in batch])
        states_ = numpy.array([no_state for o in batch])

        p = self.brain.predict(states)
        p_ = self.brain.predict(states_, target=False)
        pTarget_ = self.brain.predict(states_, target=True)

        x = numpy.zeros((len(batch), *self.state_shape))
        y = numpy.zeros((len(batch), self.action_count))

        errors = numpy.zeros(len(batch))
        for i in range(len(batch)):
            o = batch[i][1]
            s = o[0]    # State
            a = o[1]    # Action
            r = o[2]    # Reward
            s_ = o[3]   # State + 1

            t = p[i]
            old_value = t[a]

            if s_ is None:
                # Reached terminal state
                t[a] = r
            else:
                # Update according to formula
                t[a] = r + Agent.GAMMA * pTarget_[i][numpy.argmax(p_[i])]  # double DQN

            x[i] = s
            y[i] = t
            errors[i] = abs(old_value - t[a])

        return x, y, errors

    def replay(self):
        batch = self.memory.sample(Agent.BATCH_SIZE)
        x, y, errors = self._get_targets(batch)

        # Update memory weights
        for i in range(len(batch)):
            idx = batch[i][0]
            self.memory.update(idx, errors[i])

        self.brain.train(x, y)
        self.generation += 1
