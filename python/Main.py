
from game.Game import Game
from game.util import log

if __name__ == "__main__":
    logger = log.setup_custom_logger('root')



    game = Game()
    game.add_player()
    game.add_player()
    game.add_player()
    game.add_player()


    game.loop()

    print("Player %s won the game!" % game.winner)
