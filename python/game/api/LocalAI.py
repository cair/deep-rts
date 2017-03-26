

class LocalAI:

    hooks = []

    def __init__(self, player, event):
        self.player = player
        self.event = event
        #self.action = self.event.Action

    def init(self):
        for hook in LocalAI.hooks:
            hook(self)

