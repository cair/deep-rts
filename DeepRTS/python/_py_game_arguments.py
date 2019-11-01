from DeepRTS import python
from DeepRTS import Engine


class GameArguments:

    def __init__(
            self,
            game_map,
            n_players,
            engine_config,
            gui_config
    ):
        self.game_map = game_map
        self.n_player = n_players
        self.engine_config = engine_config
        self.gui_config = gui_config