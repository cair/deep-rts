import random
from Main import game
from ai.LocalAI.ChainCommand import ChainCommand

player_id = None
Action = None
tick_skip = 50
tick = 0

# Chain actions

command = None

"""".idle_worker()
.gold()
.build_worker()
.build_worker()
.idle_worker()
.gold()
.wood()
.build_worker()
.idle_worker()
.build_farm()"""
#command.generate()



def on_event(event_type, data):

    if event_type == 0:
        global tick
        tick += 1

        if command.eligible(tick):
            command.run()



    """global tick
    tick += 1
    if tick > tick_skip:
        idle_workers = Action.idle_workers()

        if idle_workers:
            possible_actions = Action.possible_actions(idle_workers[0])
            action = random.choice(possible_actions)
            action[0](*action[1])
    """

def on_defeat(data):
    pass

def on_victory(data):
    pass


player_id, Action = game.hook(
    on_victory=on_victory,
    on_defeat=on_defeat,
    on_event=on_event
)

# Build AI
command = ChainCommand(Action)

command.then_idle_worker(0)
command.then_build_town_hall(0)

command.then_idle_worker((8 * 255) + 1)
command.then_wood(0)
command.then_wood_count(0, 250)
command.then_gold(0)
command.then_gold_count(0, 500)

command.then_worker(0)
command.then_build_farm(0)

command.then_idle_worker((8 * 10000) + 1)
command.then_gold(0)

# Build new worker when enough gold
command.then_gold_count(0, 400)
command.then_idle_town_hall(0)
command.then_build_worker(0)

# New worker harvest gold
command.then_idle_worker((8 * 60) + 1)
command.then_wood(0)
command.then_wood_count(0, 250)
command.then_gold(0)
command.then_gold_count(0, 500)

command.then_worker(0)
command.then_build_farm(0)
command.then_gold(0)

for i in range(6):
    g_w = i % 2 == 0

    # Build new worker when enough gold
    command.then_gold_count(0, 400)
    command.then_idle_town_hall(0)
    command.then_build_worker(0)

    # New worker harvest_ gold
    command.then_idle_worker((8 * 60) + 1)
    command.then_gold(0) if g_w else command.then_wood(0)


command.then_idle_worker((8 * 60) + 1)
command.then_wood(0)

# Build barracks next
command.then_wood_count(0, 450)
command.then_gold_count(0, 700)
command.then_worker(0)
command.then_build_barracks(0)

command.then_idle_worker((8 * 60) + 1)
command.then_wood(0)

for i in range(2):
    command.then_idle_barracks(0)
    command.then_build_footman(0)




command.then_pause(0)
game.loop()


print("Player %s won the game!" % game.winner)
