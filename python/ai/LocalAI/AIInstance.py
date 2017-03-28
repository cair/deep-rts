

class AIInstance:

    def __init__(self, ai):
        self.ai = ai
        self.player = ai.player
        self.Event = ai.event
        self.Action = ai.action

        self.Event.on_victory(self.on_victory)
        self.Event.on_defeat(self.on_defeat)
        self.Event.on_event(self.on_event)
        self.Event.on_frame(self.on_frame)

        self.tick = 0
