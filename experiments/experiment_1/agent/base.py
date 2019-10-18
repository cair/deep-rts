

class BaseAgent:

    def __init__(self, n_actions):
        self.n_actions = n_actions

    def remember(self, state, r, t, victory):
        raise NotImplementedError("remember must be implemented!")

    def get_action(self, s):
        raise NotImplementedError("get_action must be implemented!")
