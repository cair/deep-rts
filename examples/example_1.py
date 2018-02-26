
import numpy as np
from skimage import transform, color, exposure
from examples.agent.agent import Agent
from pyDeepRTS import PyDeepRTS
if __name__ == "__main__":

    config = PyDeepRTS.DeepRTS.Config()
    config.set_instant_town_hall(False)
    g = PyDeepRTS("21x21-2v2.json", config=config, pomdp=False, simple=False)
    player1 = g.add_player()
    player2 = g.add_player()

    g.set_agent_player(player1)

    g.set_max_fps(1000000)
    g.set_max_ups(1000000)
    g.render_every(1)
    g.capture_every(1)
    g.view_every(60)
    g.start()

    ####################################################################
    #
    # Game Rules
    #
    ####################################################################
    action_frequency = 5
    num_games = 1000000

    ####################################################################
    #
    # Agent settings
    #
    ####################################################################
    agent = Agent()
    s0 = None

    for episode in range(num_games):

        counter = 0

        while True:
            counter += 1

            # Do actions
            #a = agent.get_action(s0)
            a = 0
            print("Loss: %s, | %s" % (agent.loss / agent.epoch, a))
            #player1.do_manual_action(*a)
            player1.do_action(np.random.randint(0, 16))
            player2.do_action(np.random.randint(0, 16))

            # Process game
            for _ in range(action_frequency):
                    g.tick()
                    g.update()

            # Render the game state
            g.render()
            g.view()
            g.caption()
            g.gui.capture(save=True)

            # Get and preprocess the state
            s1_image = g.get_state(True)
            s1_image = transform.resize(s1_image, (84, 84))
            s1_image = color.rgb2gray(s1_image)

            # We do this to save space
            s1_image = exposure.rescale_intensity(s1_image, out_range=(0, 255))
            s1_image = np.uint8(s1_image)

            s1_features = np.array([
                player1.gold(),
                player1.lumber(),
                player1.sGatheredGold,
                player1.sGatheredLumber,
                player1.sDamageDone,
                player1.sDamageTaken,
                player1.sUnitsCreated
            ])

            s1 = [np.reshape(s1_image, (1, 84, 84, 1)), np.expand_dims(s1_features, 0)]
            r = -0.01
            t = False

            if g.is_terminal():
                g.reset()
                print("Game %s - %s" % (episode, counter))

                if player1.is_defeated():
                    r = -1
                else:
                    r = 1

                t = True

            if s0:
                agent.memory.add((s0, a, r, s1, t))
            s0 = s1







