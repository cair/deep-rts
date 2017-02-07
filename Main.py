# https://www.spriters-resource.com/pc_computer/warcraft2/sheet/29501/
from game.WarC2 import Game
from game.util import log
logger = log.setup_custom_logger('root')


LOAD_SAVED_GAME = False


if LOAD_SAVED_GAME:

    game = Game.load()

else:

    select_map = "simple"
    select_players = 2
    game = Game.start(
        map_name=select_map,
        players=select_players
    )


if __name__ == "__main__":
    # Start loop
    game.loop()

    print("Player %s won the game!" % game.winner)
