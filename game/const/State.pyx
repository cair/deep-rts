
ID_Idle = 0
ID_Building = 1
ID_Harvesting = 2
ID_Combat = 3
ID_Walking = 4
ID_Dead = 5
ID_Spawning = 6
ID_Despawned = 7

"""from game.state.Idle import Idle
from game.state.Building import Building
from game.state.Harvesting import Harvesting
from game.state.Combat import Combat
from game.state.Walking import Walking
from game.state.Dead import Dead
from game.state.Spawning import Spawning
from game.state.Despawned import Despawned
"""
"""
mapping = {
    ID_Idle: Idle,
    ID_Building: Building,
    ID_Harvesting: Harvesting,
    ID_Combat: Combat,
    ID_Walking: Walking,
    ID_Dead: Dead,
    ID_Spawning: Spawning,
    ID_Despawned: Despawned
}
"""

def new(unit, state_clazz, properties={}):
    state = state_clazz(unit)

    # Set properties inserted as attributes
    for key in properties:
        setattr(state, key, properties[key])

    state.init()
    return state

