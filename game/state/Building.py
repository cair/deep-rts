from game.logic.UnitManager import UnitManager
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
    target_id = None
    entity_id = None  # Set by init()

    def __init__(self, unit, attributes={}):
        super().__init__(unit, attributes)

    def toJSON2(self):
        return {
            'build_timer': self.build_timer,
            'build_duration': self.build_duration,
            'target_id': self.target_id,
            'entity_id': self.entity_id
        }

    def init(self):
        target_clazz = UnitManager.get_class_by_id(self.target_id)

        self.build_timer = 0
        self.build_duration = target_clazz.build_duration
        # Create initial instance of unit

        entity = target_clazz(self.unit.player)
        entity.add_to_game()
        if not self.unit.structure:
            entity.state.x = self.unit.state.x - entity.dimension
            entity.state.y = self.unit.state.y - entity.dimension
            self.unit.despawn()
            entity.spawn()

        self.entity_id = entity.unit_id

    def spawn_subject(self, unit1, unit2):
        """
        This functions spawns a unit (unit1) given unit2's position
        :param unit1:
        :param unit2:
        :return: Returns True if successful
        """

        adjacent_tiles = self.game.AdjacentMap.adjacent_walkable(self.game,
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
            entity = self.game.units[self.entity_id]
            entity.state.direction = UP

            did_spawn = False
            if not self.unit.state.spawned:
                # Builder is despawned, Spawn unit adjacent to newly built entity
                did_spawn = self.spawn_subject(entity, self.unit)
            elif not entity.state.spawned:
                # Entity is despawned, Spawn entity (for example TownHall spawns Peasant)
                did_spawn = self.spawn_subject(self.unit, entity)

            if did_spawn:
                entity.state.transition()
                self.transition()

# Register to Generic State
GenericState.registry[Building.type] = Building