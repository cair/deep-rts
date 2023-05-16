from DeepRTS.contrib.agents.agent import BaseAgent
from DeepRTS.Engine import Random


class RandomAgent(BaseAgent):

    def __init__(self, env):
        super(RandomAgent, self).__init__(env=env)

    def get_state(self):
        return self.env.get_state(image=False, copy=False)

    def get_action(self, obs):
        return Random.action() - 1

    def state_preprocess(self, obs):
        return obs
        # return obs.swapaxes(0, 1).copy()

    def memorize(self, obs, obs1, action, reward, terminal, info):
        pass
