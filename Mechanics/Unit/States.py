class State:

    def __init__(self, player):
        self.player = player

    def transition(self, new_state):
        print("Transition from %s => %s" % (self.id, new_state.id))
        self.player.state = new_state

class Spawning(State):
    id = "Spawning"
    def __init__(self, player):
        super().__init__(player)
        pass

class Despawned(State):
    id = "Despawned"
    def __init__(self, player):
        super().__init__(player)

class Walking(State):

    def __init__(self, player):
        super().__init__(player)


class Combat(State):

    def __init__(self, player):
        super().__init__(player)



class Idle(State):

    def __init__(self, player):
        super().__init__(player)


class Dead(State):

    def __init__(self, player):
        super().__init__(player)


class Harvesting(State):

    def __init__(self, player):
        super().__init__(player)



class Building(State):

    def __init__(self, player):
        super().__init__(player)
