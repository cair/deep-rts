# https://www.spriters-resource.com/pc_computer/warcraft2/sheet/29501/
from WarC2 import Game
import log
logger = log.setup_custom_logger('root')

# Arguments
select_map = "simple"
select_players = 2


# Create game instance
game = Game(
    map_name=select_map,
    players=select_players
)

if __name__ == "__main__":
    # Start loop
    game.loop()

    print("Player %s won the game!" % game.winner)
