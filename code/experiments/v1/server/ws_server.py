import json
import pickle
import numpy as np
from multiprocessing import Process
from flask import Flask, render_template
from flask_socketio import SocketIO

from v1.server.database import GameResult


class WebsocketServer(Process):

    def __init__(self, raw_o_q, w_in_q):
        Process.__init__(self)
        self.app = Flask(__name__)
        self.app.config['SECRET_KEY'] = 'secret!'
        self.socketio = SocketIO(self.app, logger=False)
        self.connected_users = 0
        self.raw_o_q = raw_o_q
        self.w_in_q = w_in_q

        print("[x] - Getting initial generation, weights and memory")
        loss, generation, weights, memory = self.w_in_q.get()
        self.latest_weights = weights
        self.latest_generation = generation
        self.latest_loss = loss
        print("[x] - Done!")

        self.socketio.on_event("observe", self.observe, namespace="/")
        self.socketio.on_event("game_result", self.game_result, namespace="/")
        self.socketio.on_event("game_results", self.game_results, namespace="/")
        self.socketio.on_event("connect", self.connected, namespace="/")
        self.socketio.on_event("disconnect", self.disconnect, namespace="/")
        self.socketio.on_error_default(self.default_error_handler)
        self.app.add_url_rule("/get_weights", "get_weights", self.get_weights)
        self.app.add_url_rule("/get_current_generation", "get_current_generation", self.get_current_generation)
        self.app.add_url_rule("/", "index", self.index)

    def index(self):
        return render_template('index.html')

    def get_weights(self):
        pickled = pickle.dumps((self.latest_loss, self.latest_generation, self.latest_weights))
        return pickled

    def get_current_generation(self):
        return json.dumps({"generation": self.latest_generation})

    def observe(self, data):
        data = json.loads(data)
        s0 = np.array(data[0])
        a = data[1]
        r = data[2]
        t = data[3]
        s1 = np.array(data[4])

        self.raw_o_q.put((s0, a, r, t, s1))

        if not self.w_in_q.empty():
            loss, generation, weights, memory = self.w_in_q.get()
            self.latest_weights = weights
            self.latest_generation = generation
            self.latest_loss = loss
            print("Updated WebsocketServer weights (Gen: %s)" % generation)
            self.socketio.emit("weight_status", json.dumps({
                "generation": self.latest_generation
            }))

    def game_results(self, data):
        items = [{
            "game_id": x.game_id,
            "generation": x.generation,
            "checksum": x.checksum,
            "loss": x.loss,
            "score": x.score,
            "victory": x.victory
        } for x in GameResult.select()]

        self.socketio.emit("web_game_results", json.dumps(items))

    def game_result(self, data):
        self.socketio.emit("web_game_result", data)
        data = json.loads(data)
        x = GameResult(game_id=data["game_id"],
                       generation=data["generation"],
                       checksum=data["checksum"],
                       loss=data["loss"],
                       score=data["score"],
                       victory=data["victory"]
                       )
        x.save()

    def connected(self):
        self.connected_users += 1
        print("Client Connected!")

    def disconnect(self):
        self.connected_users -= 1
        print("Client Disconnected!")

    def default_error_handler(self, e):
        print(e)

    def run(self):
        print("Starting ws loop")
        self.socketio.run(self.app, port=8080)


