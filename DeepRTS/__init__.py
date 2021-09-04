try:
    from DeepRTS import Engine
except ImportError:
    import Engine

try:
    from DeepRTS.Engine import Map, UnitManager, Constants, Player
    from DeepRTS.Engine import Constants
except ImportError:
    from Engine import Map, UnitManager, Constants, Player, Constants

