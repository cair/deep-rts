from game.api.Action import Action


class Event:
    from game.const.Event import NEW_STATE
    handles = []   # List of all hooked Event instances (All players) Used for broadcasts
    c_end = None
    c_start = None
    c_defeat = None
    c_victory = None
    c_event = None

    def __init__(self, player):
        Event.handles.append(self)
        self.Action = Action(player)


# Callback setters

    def on_start(self, callback):
        self.c_start = callback

    def on_end(self, callback):
        self.c_end = callback

    def on_defeat(self, callback):
        self.c_defeat = callback

    def on_victory(self, callback):
        self.c_victory = callback

    def on_event(self, callback):
        self.c_event = callback

    # Notifiers


    def notify(self, event_type, data=None):
        if not self.c_event: return
        self.c_event(event_type, data)

    @staticmethod
    def notify_broadcast(event_type, data=None):
        for e in Event.handles:
            e.notify(event_type, data)

    def notify_victory(self, data):
        if not self.c_victory: return
        self.c_victory(data)

    def notify_defeat(self, data):
        if not self.c_defeat: return
        self.c_defeat(data)

    def notify_start(self, data):
        if not self.c_start: return
        self.c_start(data)

    def notify_end(self):
        if not self.c_end: return
        self.c_end()
