from game.state.GenericState import GenericState
from game import const

class Building(GenericState):
    id = "Building"
    type = const.State.ID_Building

    def __init__(self, unit):
        super().__init__(unit)
        self.data = self.unit.d['s'][Building.type]

    def init(self):

        target_clazz = self.player.UnitManager.get_class_by_id(self.data['target_id'])

        # Create initial instance of unit
        entity = target_clazz(self.unit.player)
        entity.add_to_game()
        if self.unit.structure == const.Unit.TYPE_UNIT:
            entity.d['x'] = self.unit.d['x'] - entity.dimension
            entity.d['y'] = self.unit.d['y'] - entity.dimension
            self.unit.despawn()
            entity.spawn()

        self.data['entity_id'] = entity.unit_id

    def spawn_subject(self, unit1, unit2):
        """
        This functions spawns a unit (unit1) given unit2's position
        :param unit1:
        :param unit2:
        :return: Returns True if successful
        """

        adjacent_tiles = self.game.AdjacentMap.adjacent_walkable(self.game,
            unit1.d['x'] + unit1.dimension,
            unit1.d['y'] + unit1.dimension,
            unit1.dimension + 1
        )

        if not adjacent_tiles:
            return False

        unit2.d['x'], unit2.d['y'] = adjacent_tiles.pop()
        unit2.spawn()

        return True

    def update(self, tick):

        self.data['build_timer'] += tick

        #print(self.game.units, self.data['build_timer'], self.data['build_duration'], self.data['build_timer'] >= self.data['build_duration'])
        if self.data['build_timer'] >= self.data['build_duration']:
            # Build is done
            entity = self.game.units[self.data['entity_id']]
            entity.d['direction'] = const.Unit.DIRECTION_UP

            did_spawn = False
            if not self.unit.d['spawned']:
                # Builder is despawned, Spawn unit adjacent to newly built entity
                did_spawn = self.spawn_subject(entity, self.unit)
            elif not entity.d['spawned']:
                # Entity is despawned, Spawn entity (for example TownHall spawns Peasant)
                did_spawn = self.spawn_subject(self.unit, entity)

            if did_spawn:

                entity.state.transition()
                self.transition()
                #self.data['entity_id'] = None
                #self.data['build_timer'] = 0
