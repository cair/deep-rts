
# Run setup.py install in sources/python to build source files.
from DeepRTS.python import scenario
from DeepRTS.Engine import Random
from DeepRTS.python.scenario.engine import Scenario
from DeepRTS.python import util, Config, Game
from DeepRTS import Engine

class NewScenario(Scenario):

    def __init__(self, config):

        engconf = config["engine"] if "engine" in config else {}
        gconf = config["gui"] if "gui" in config else {}
        rlconf = config["rl"] if "rl" in config else {}

        gui_config = Config(
            render=util.config(gconf, "render", True),
            view=util.config(gconf, "view", True),
            inputs=util.config(gconf, "inputs", False),
            caption=util.config(gconf, "caption", False),
            unit_health=util.config(gconf, "unit_health", True),
            unit_outline=util.config(gconf, "unit_outline", True),
            unit_animation=util.config(gconf, "unit_animation", False),
            audio=util.config(gconf, "audio", False),
            audio_volume=util.config(gconf, "audio_volume", 50)
        )

        engine_config: Engine.Config = Engine.Config.defaults()
        engine_config.set_barracks(True)
        engine_config.set_footman(True)
        engine_config.set_instant_town_hall(True)
        engine_config.set_archer(True)
        engine_config.set_start_lumber(250)  # Enough to create TownHall
        engine_config.set_start_gold(500)  # Enough to create TownHall
        engine_config.set_start_oil(0)
        engine_config.set_tick_modifier(util.config(engconf, "tick_modifier", engine_config.tick_modifier))

        game = Game(
            Config.Map.FIFTEEN,
            n_players=2,
            engine_config=engine_config,
            gui_config=gui_config,
            terminal_signal=False
        )

        c_fps = engconf["fps"] if "fps" in engconf else -1
        c_ups = engconf["ups"] if "ups" in engconf else -1

        game.set_max_fps(c_fps)
        game.set_max_ups(c_ups)

        super().__init__(
            rlconf,
            game,
            #Scenario.GOLD_COLLECT(1000),
            Scenario.GOLD_COLLECT_INCREMENT(100)
        )

    def _optimal_play_sequence(self):
        return [
            (Engine.Constants.Action.MoveRight, "Peasant0"),
            (Engine.Constants.Action.MoveRight, "Peasant0"),
            (Engine.Constants.Action.MoveRight, "Peasant0"),
            (Engine.Constants.Action.MoveRight, "Peasant0"),
            (Engine.Constants.Action.MoveDownRight, "Peasant0"),
            (Engine.Constants.Action.MoveRight, "Peasant0")
        ]

if __name__ == "__main__":
    episodes = 10
    random_play = True

    env = NewScenario({})

    env.game.set_max_fps(120)
    env.game.set_max_ups(120)

    scores = []

    for episode in range(episodes):
        print("Episode: %s, FPS: %s, UPS: %s" % (episode, env.game.get_fps(), env.game.get_ups()), flush = True)

        terminal = False
        state = env.reset()
        score = 0

        while not terminal:

            action = Random.action() - 1  # TODO AI Goes here
            next_state, reward, terminal, _ = env.step(action)

            score += reward

        scores.append(score)

    print(scores)






