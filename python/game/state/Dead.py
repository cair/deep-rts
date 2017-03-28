from game import Constants


class Dead:
    name = "Dead"
    id = Constants.State.Dead



    def update(self, unit):
        pass

    def end(self, unit):
        pass

    def init(self, unit):
        self.unit.despawn()
        self.unit.remove_from_game()