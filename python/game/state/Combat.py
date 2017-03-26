from game import Config
from game import Constants


class Combat:
    name = "Combat"
    id = Constants.State.Combat

    def __init__(self):
        pass

    def update(self, unit):
        self.data['attack_timer'] += tick

        if self.data['attack_timer'] >= self.attack_interval:
            attack_target = self.game.units[self.data['attack_target']]

            distance = self.unit.distance(
                attack_target.d['x'],
                attack_target.d['y'],
                attack_target.dimension
            )
            if distance > self.unit.range:
                # Too far away, Walk

                # Find adjacent tile to the attack target
                tiles = self.game.AdjacentMap.adjacent_walkable(
                    self.game,
                    attack_target.d['x'] + attack_target.dimension,
                    attack_target.d['y'] + attack_target.dimension,
                    attack_target.dimension + 1
                )
                # No tiles are available around the target
                if not tiles:
                    return

                # Find the tile with shortest distance
                tile = self.unit.shortest_distance(tiles)

                # Next state is Combat (After movement)
                self.add_next(Combat.type, {
                    'attack_target': self.data['attack_target']
                }, 1)

                # Move to tile (This triggers transition)
                self.unit.move(*tile)
            else:
                # Can attack (Distance < Range)
                my_damage = self.unit.get_damage(self.unit)
                attack_target.afflict_damage(my_damage)

                # If attack target is dead, transition to next state
                if attack_target.is_dead():
                    self.data['attack_target'] = None
                    self.player.statistics['kill_count'] += 1
                    self.transition()
                    return

                # Make unit retaliate if its Idle (Changes to attack state)
                if attack_target.state.type == const.State.ID_Idle:
                    attack_target.attack(self.unit.d['x'], self.unit.d['y'])

            self.data['attack_timer'] = 0

    def end(self, unit):
        pass

    def init(self, unit):
        pass