import time
import multiprocessing
from v1.experiment_1 import Agent
from v1.server.observe_server import ObserveServer
from v1.server.train_server import TrainServer
from v1.server.ws_server import WebsocketServer

STATE_SIZE = (4, 10, 10)
ACTION_SIZE = 16

agent = Agent(state_shape=STATE_SIZE, action_count=ACTION_SIZE)

raw_o_q = multiprocessing.Queue()
processed_o_q = multiprocessing.Queue()
w_q_0 = multiprocessing.Queue()
w_q_1 = multiprocessing.Queue()

o_s = ObserveServer(raw_o_q, processed_o_q, w_q_0, agent)
o_s.start()

t_s = TrainServer(agent, processed_o_q, (w_q_0, w_q_1))
t_s.start()

ws_s = WebsocketServer(raw_o_q, w_q_1)
ws_s.start()

while True:
    time.sleep(1)