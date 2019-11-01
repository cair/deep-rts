from DeepRTS import python
from DeepRTS import Engine


class GameArguments:

    def __init__(
            self,
            game_map: python.Config.Map,
            n_players: int,
            engine_config: Engine.Config,
            gui_config: python.Config
    ):
        self.game_map = game_map
        self.n_player = n_players
        self.engine_config = engine_config
        self.gui_config = gui_config