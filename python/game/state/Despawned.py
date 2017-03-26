from game import Constants


class Despawned:
    name = "Despawned"
    id = Constants.State.Despawned

    def update(self, unit):
        if not self.done:
            self.unit.despawn()

    def end(self, unit):
        pass

    def init(self, unit):
        pass