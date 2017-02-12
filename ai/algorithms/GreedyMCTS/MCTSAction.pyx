
from game.loaders.MapLoader import MapLoader

from game.const.cConst import *



cdef class MCTSAction:



    @staticmethod
    def available_actions(player):
        action_set = []
        game = player.game

        idle_units = []                             # Define a list for idle units
        for u_id in player.units:                   # Iterate through all of the users units
            unit = player.game.units[u_id]          # Locate unit object in game unit dictionary
            if unit.state.type == 0:                # If unit is idle  0 IS IDLE
                idle_units.append(unit)             # Append to idle_unit list

        for unit in idle_units:
            AM = unit.AdjacentMap
            uid = unit.unit_id


            tiles = AM.adjacent(unit.state.x + unit.dimension, unit.state.y + unit.dimension, unit.dimension + 1)

            # Buildable structures
            for i, b in enumerate(unit.buildable):
                if unit.can_build(b) and unit.can_build_here(b):
                    action_set.append(('build', uid, 2, [i])) # 2 is BUILD

            for x, y in tiles:
                is_grass = MapLoader.tiles[x][y] == MAP_GRASS
                is_no_unit = game.data['unit'][x, y] == UNIT_NONE
                is_harvestable = MapLoader.tiles[x][y]in [MAP_WOOD, MAP_GOLD]
                is_attackable = unit.game.data['unit_pid'][x, y] not in [unit.player.id]

                if is_grass and is_no_unit and unit.structure == UNIT_UNIT:
                    action_set.append(('move', uid, 0, (x, y)))  # 0 is RIGHT_CLICK

                if is_harvestable and unit.can_harvest:
                    action_set.append(('harvest', uid, 0, (x, y)))

                if is_attackable and unit.damage_min > 0:
                    action_set.append(('attack', uid, 0, (x, y)))

                    # 3 IS VOID
        return action_set

