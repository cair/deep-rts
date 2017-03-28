from game import Constants
from game import state


class StateManager:

    walking_state = state.Building()
    spawn_state = state.Spawning()
    idle_state = state.Idle()
    despawned_state = state.Despawned()
    harvesting_state = state.Harvesting()
    building_state = state.Building()
    combat_state = state.Combat()
    dead_state = state.Dead()

    @staticmethod
    def get_by_id(state_type_id):
        if state_type_id == Constants.State.Building:
            return StateManager.walking_state
        elif state_type_id == Constants.State.Spawning:
            return StateManager.spawn_state
        elif state_type_id == Constants.State.Walking:
            return StateManager.walking_state
        elif state_type_id == Constants.State.Idle:
            return StateManager.idle_state
        elif state_type_id == Constants.State.Despawned:
            return StateManager.despawned_state
        elif state_type_id == Constants.State.Harvesting:
            return StateManager.harvesting_state
        elif state_type_id == Constants.State.Combat:
            return StateManager.combat_state
        elif state_type_id == Constants.State.Dead:
            return StateManager.dead_state
        else:
            assert False  # Should NEVER happen