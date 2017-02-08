from game import Config
from game.state.GenericState import GenericState
from game.const.State import ID_Combat
from game.state.Idle import Idle


class Combat(GenericState):
    id = "Combat"
    type = ID_Combat
    default = Idle

    attack_interval = 1.25 * Config.FRAME_MULTIPLIER

    attack_target = None
    attack_timer = 1000

    def __init__(self, unit, attributes={}):
        super().__init__(unit, attributes)

    def toJSON2(self):
        return {
            'attack_target': self.attack_target,
            'attack_timer': self.attack_timer
        }

    def update(self, tick):
        self.attack_timer += tick

        if self.attack_timer >= self.attack_interval:
            distance = self.unit.distance(
                self.attack_target.state.x,
                self.attack_target.state.y,
                self.attack_target.dimension
            )
            if distance > self.unit.range:
                # Too far away, Walk

                # Find adjacent tile to the attack target
                tiles = self.Map.AdjacentMap.adjacent_walkable(
                    self.attack_target.state.x + self.attack_target.dimension,
                    self.attack_target.state.y + self.attack_target.dimension,
                    self.attack_target.dimension + 1
                )
                # No tiles are available around the target
                if not tiles:
                    return

                # Find the tile with shortest distance
                tile = self.unit.shortest_distance(tiles)

                # Next state is Combat (After movement)
                self.add_next(Combat, {
                    'attack_target': self.attack_target
                }, 1)

                # Move to tile (This triggers transition)
                self.unit.move(*tile)
            else:
                # Can attack (Distance < Range)
                print(self.health)
                my_damage = self.unit.get_damage(self.unit)
                self.attack_target.afflict_damage(my_damage)

                # If attack target is dead, transition to next state
                if self.attack_target.is_dead():

                    self.player.statistics['kill_count'] += 1
                    self.transition()
                    return

                # Make unit retaliate if its Idle (Changes to attack state)
                if type(self.attack_target.state) == Idle:
                    self.attack_target.attack(self.unit.state.x, self.unit.state.y)

            self.attack_timer = 0

# Register to Generic State
GenericState.registry[Combat.type] = Combat