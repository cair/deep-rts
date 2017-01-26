# https://www.spriters-resource.com/pc_computer/warcraft2/sheet/29501/
import logging
import virtualtime
virtualtime.enable()

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

clock = GameClock(
    max_fps=0,
    max_ups=1000,
    use_wait=True
)

g = Game(map_name=select_map, players=select_players, clock=clock)

clock.update_callback = g.process,
clock.frame_callback = g.draw

clock.schedule_interval(g.gui.caption, 1.0)
clock.use_wait = False


while Config.IS_RUNNING:
    clock.tick()



