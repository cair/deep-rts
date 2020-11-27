
# Run setup.py install in sources/python to build source files.

from DeepRTS.python import scenario
from DeepRTS.Engine import Random
if __name__ == "__main__":
    episodes = 10
    random_play = True

    env = scenario.GoldCollectOnePlayerFifteen({})

    env.game.set_max_fps(99999999)
    env.game.set_max_ups(99999999)

    env.calculate_optimal_play()

    for episode in range(episodes):
        print("Episode: %s, FPS: %s, UPS: %s" % (episode, env.game.get_fps(), env.game.get_ups()))

        terminal = False
        state = env.reset()

        while not terminal:

            action = Random.action() - 1  # TODO AI Goes here
            next_state, reward, terminal, _ = env.step(action)






