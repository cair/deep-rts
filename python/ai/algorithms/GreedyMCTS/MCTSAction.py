
from game import const



class MCTSAction:



    @staticmethod
    def available_actions(player):
        action_set = []
        game = player.game

        idle_units = []                             # Define a list for idle units
        for u_id in player.units:                   # Iterate through all of the users units
            unit = player.game.units[u_id]          # Locate unit object in game unit dictionary
            if unit.state.type == const.State.ID_Idle:                # If unit is idle  0 IS IDLE
                idle_units.append(unit)             # Append to idle_unit list

        for unit in idle_units:
            AM = unit.AdjacentMap
            uid = unit.unit_id

            tiles = AM.adjacent(unit.d['x'] + unit.dimension, unit.d['y'] + unit.dimension, unit.dimension + 1)


            # Buildable structures
            for i, b in enumerate(unit.buildable):
                if unit.can_build(b) and unit.can_build_here(b):
                    action_set.append(('build', uid, 2, [i])) # 2 is BUILD

            for x, y in tiles:
                is_grass = player.game.Map.tiles[x][y] == const.Map.ID_GRASS
                is_no_unit = game.data['unit'][x, y] == const.Unit.ID_NONE
                is_harvestable = player.game.Map.tiles[x][y]in [const.Map.ID_WOOD, const.Map.ID_GOLD]
                is_attackable = unit.game.data['unit_pid'][x, y] not in [unit.player.id]

                if is_grass and is_no_unit and unit.structure == const.Unit.TYPE_UNIT:
                    action_set.append(('move', uid, 0, (x, y)))  # 0 is RIGHT_CLICK

                if is_harvestable and unit.can_harvest:
                    action_set.append(('harvest', uid, 0, (x, y)))

                if is_attackable and unit.damage_min > 0:
                    action_set.append(('attack', uid, 0, (x, y)))

                    # 3 IS VOID
        return action_set

