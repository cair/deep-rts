import gym
import numpy as np


class ImageToPyTorch(gym.ObservationWrapper):
    def __init__(self, env):
        super(ImageToPyTorch, self).__init__(env)
        self.original_shape = self.observation_space.shape
        self.observation_space = gym.spaces.Box(
            low=0.0,
            high=1.0,
            shape=(
                self.original_shape[-1],
                self.original_shape[0],
                self.original_shape[1]
            ),
            dtype=np.float32
        )

    def observation(self, observation):
        return np.moveaxis(observation, 2, 0)