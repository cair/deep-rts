from game.state.GenericState import GenericState
from game.const.State import ID_Building
from game.state.Idle import Idle
from game.const.Unit import UP

class Building(GenericState):
    id = "Building"
    type = ID_Building
    default = Idle

    build_timer = 20
    build_duration = 20
    target = None
    entity = None  # Set by init()

    def __init__(self, unit, attributes={}):
        super().__init__(unit, attributes)

    def toJSON2(self):
        return {
            'build_timer': self.build_timer,
            'build_duration': self.build_duration,
            'target': self.target,
            'entity': self.entity
        }

    def init(self):
        self.build_timer = 0
        self.build_duration = self.target.build_duration
        # Create initial instance of unit
        entity = self.target(self.unit.player)
        entity.add_to_game()
        if not self.unit.structure:
            entity.state.x = self.unit.state.x - entity.dimension
            entity.state.y = self.unit.state.y - entity.dimension
            self.unit.despawn()
            entity.spawn()

        self.entity = entity

    def spawn_subject(self, unit1, unit2):
        """
        This functions spawns a unit (unit1) given unit2's position
        :param unit1:
        :param unit2:
        :return: Returns True if successful
        """

        adjacent_tiles = self.game.Map.AdjacentMap.adjacent_walkable(
            unit1.state.x + unit1.dimension,
            unit1.state.y + unit1.dimension,
            unit1.dimension + 1
        )

        if not adjacent_tiles:
            return False

        unit2.state.x, unit2.state.y = adjacent_tiles.pop()
        unit2.spawn()

        return True

    def update(self, tick):
        self.build_timer += tick

        if self.build_timer >= self.build_duration:
            # Build is done
            self.entity.direction = UP

            did_spawn = False
            if not self.unit.state.spawned:
                # Builder is despawned, Spawn unit adjacent to newly built entity
                did_spawn = self.spawn_subject(self.entity, self.unit)
            elif not self.entity.state.spawned:
                # Entity is despawned, Spawn entity (for example TownHall spawns Peasant)
                did_spawn = self.spawn_subject(self.unit, self.entity)

            if did_spawn:
                self.entity.state.transition()
                self.transition()
