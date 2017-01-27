# https://www.spriters-resource.com/pc_computer/warcraft2/sheet/29501/
import logging

from AI.HardCoded.AI_1.AI import AI

logging.basicConfig(level=logging.DEBUG)
from WarC2 import Game
from Mechanics.Constants import Config
from GameClock import GameClock
import threading
import time


#def speed_time():
    #virtualtime.fast_forward_time(1000, step_size=1)

#t1 = threading.Thread(target=speed_time)
#t1.start()




select_map = "simple"
select_players = 2

clock = GameClock()

g = Game(map_name=select_map, players=select_players, clock=clock)

# Set AI for AI xD
for player in g.players[1:]:
    ai = AI(player)

clock.shedule(g.gui.caption, 1.0)
clock.update(g.process, 16)  # 16
clock.render(g.draw, 60) # 60

while Config.IS_RUNNING:
    clock.tick()



