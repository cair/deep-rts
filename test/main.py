
# Run setup.py install in sources/python to build source files.
import random
from DeepRTS import DeepRTSGame
if __name__ == "__main__":

    episodes = 1000
    game = DeepRTSGame('15x15-2v2.json', n_players=2)
    game.set_max_fps(1000000000)
    game.set_max_ups(1000000000)

    for episode in range(episodes):
        print(f"Episode: {episode}, FPS: {game.get_fps()}, UPS: {game.get_ups()}")

        while not game.is_terminal():

            game.tick()
            game.update()
            game.render()
            game.caption()
            #game.view()

            for player in game.players:
                player.do_action(random.randint(0, 25))


        game.reset()








