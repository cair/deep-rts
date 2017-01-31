import random
from Main import game

player_id = None
Action = None
tick_skip = 50
tick = 0

def on_event(event_type, data):

    global tick
    tick += 1
    if tick > tick_skip:
        idle_workers = Action.idle_workers()

        if idle_workers:
            possible_actions = Action.possible_actions(idle_workers[0])
            action = random.choice(possible_actions)
            action[0](*action[1])

def on_defeat(data):
    pass

def on_victory(data):
    pass


player_id, Action = game.hook(
    on_victory=on_victory,
    on_defeat=on_defeat,
    on_event=on_event
)

game.loop()


print("Player %s won the game!" % game.winner)
