# https://www.spriters-resource.com/pc_computer/warcraft2/sheet/29501/
import logging
logging.basicConfig(level=logging.DEBUG)

from WarC2 import Game
from Mechanics.Constants import Config

# Arguments
select_map = "simple"
select_players = 2

# Create game instance
game = Game(
    map_name=select_map,
    players=select_players
)



