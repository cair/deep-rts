

class MCTSAction:

    IDLE = 0

    # Action Unit
    RIGHT_CLICK = 0
    LEFT_CLICK = 1
    BUILD = 2
    VOID = 3


    @staticmethod
    def available_actions(player):
        data = []
        game = player.game

        idle_units = []                             # Define a list for idle units
        for u_id in player.units:                   # Iterate through all of the users units
            unit = player.game.units[u_id]          # Locate unit object in game unit dictionary
            if unit.state.type == MCTSAction.IDLE:          # If unit is idle
                idle_units.append(unit)             # Append to idle_unit list

        for unit in idle_units:
            AM = unit.AdjacentMap
            uid = unit.unit_id

            action_set = list()

            # Walkable tiles (1x radius)
            if unit.structure:
                adj_tiles = []
            else:
                adj_tiles = AM.adjacent_walkable(           # Here we are finding all tiles that the unit can walk on
                    game,
                    unit.state.x + unit.dimension,
                    unit.state.y + unit.dimension,
                    unit.dimension + 1
                )

            # Walkable tiles (1x radius)
            if not unit.can_harvest:
                adj_harvest = []
            else:
                adj_harvest = AM.adjacent_harvestable(      # Here we are finding all tiles that the unit can walk on
                    unit
                )

            # Walkable tiles (1x radius)
            if unit.damage_range == 0:
                adj_attack = []
            else:
                adj_attack = AM.adjacent_attackable(      # Here we are finding all tiles that the unit can walk on
                    unit
                )

            # Buildable structures
            buildables = [i for i, b in enumerate(unit.buildable) if unit.can_build(b) and unit.can_build_here(b)]


            # Idle action (Right click on yourself)
            move_action = [('move', uid, MCTSAction.RIGHT_CLICK, x) for x in adj_tiles]
            idle_action = [] #[('idle', uid, MCTSAction.VOID, ())]
            build_action = [('build', uid, MCTSAction.BUILD, [i]) for i in buildables]
            attack_action = [('attack', uid, MCTSAction.RIGHT_CLICK, x) for x in adj_attack]
            harvest_action = [('harvest', uid, MCTSAction.RIGHT_CLICK, x) for x in adj_harvest]

            # Merge all actions together
            all_actions = move_action + idle_action + build_action + attack_action + harvest_action

            # Create a action_table of all the moves

            data += all_actions

        return data

