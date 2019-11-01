import json
import threading

import pickle
import requests
import time
from socketIO_client import SocketIO, BaseNamespace


class Namespace(BaseNamespace):
    def on_connect(self):
        print('[Connected]')

    def on_reconnect(self):
        print('[Reconnected]')

    def on_disconnect(self):
        print('[Disconnected]')


class DataClient:
    def _receive_events_thread(self):
        self.socket.wait()

    def __init__(self, game_id=None, agent=None):
        print("Trying to connect to master server!")
        self.socket = SocketIO("localhost", 8080, Namespace)

        self.game_id = game_id
        self.weight_hash = None
        self.weight_generation = 0
        self.weight_loss = None
        self.weight_update = True
        self.agent = agent
        self.socket.on("weight_status", self.weight_status)
        t = threading.Thread(target=self._receive_events_thread)
        #t.daemon = True
        t.start()
        print("Connected to master server!")
        self.update_weights()

    def update_weights(self):
        s = time.time()
        current_generation = requests.get("http://localhost:8080/get_current_generation").json()
        if current_generation["generation"] >= self.weight_generation + 50:
            pickled = requests.get("http://localhost:8080/get_weights").content
            loss, generation, weights = pickle.loads(pickled)
            self.weight_hash = hash(pickled)
            self.weight_generation = generation
            self.weight_loss = loss

            self.agent.generation = self.weight_generation
            self.agent.brain.loss = self.weight_loss

            self.agent.brain.set_weights(weights)

            print("Weight update request took %s" % (time.time() - s))
            self.weight_update = False


    def weight_status(self, data):
        # This is actually in separate thread, so it cannot update self objects
        data = json.loads(data)
        print("Server reports weight generation %s" % data["generation"])
        #self.weight_generation = data["generation"]
        #self.weight_update = True

    def post_result(self, score, victory):
        if self.weight_loss is None and self.weight_generation is None and self.weight_hash is None:
            print("Skipping posting results because it agent has not yet received weights from server")
            return

        self.socket.emit("game_result", json.dumps({
            "game_id": self.game_id,
            "generation": self.agent.generation,
            "checksum": self.weight_hash,
            "loss": self.agent.brain.loss,
            "score": score,
            "victory": victory
        }))

    def observe(self, s0, a, r, t, s1):
        _s0 = s0.tolist()
        _s1 = s1.tolist()

        self.socket.emit("observe", json.dumps([_s0, a, r, t, _s1]), )
