import json
import pickle
import queue

import datetime
import peewee
from flask import Flask, render_template
from flask_socketio import SocketIO
from v1.experiment_1 import Agent
import numpy as np
import eventlet
import logging
logging.basicConfig(level=logging.WARN)
logger = logging.getLogger('data_server')
eventlet.monkey_patch()

############################################################
#
# Database
#
############################################################
db = peewee.SqliteDatabase('data_server.db')


class Memory(peewee.Model):
    id = peewee.PrimaryKeyField()
    data = peewee.BlobField()
    generation = peewee.IntegerField()
    date = peewee.DateTimeField(default=datetime.datetime.now)

    class Meta:
        database = db


class Weights(peewee.Model):
    id = peewee.PrimaryKeyField()
    data = peewee.BlobField()
    generation = peewee.IntegerField()
    date = peewee.DateTimeField(default=datetime.datetime.now)

    class Meta:
        database = db


try:
    db.create_tables([Memory, Weights])
except peewee.OperationalError:
    pass


############################################################
#
# Flask SocketIO + Agent
#
############################################################

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app, logger=False)
connected_users = 0

STATE_SIZE = (4, 10, 10)
ACTION_SIZE = 16
GENERATION_SAVE_INTERVAL = 50

agent = Agent(state_shape=STATE_SIZE, action_count=ACTION_SIZE)
memory_queue = queue.Queue()

# Load latest generation of weights
try:
    weights = Weights.select().order_by(Weights.id.desc()).get()
    agent.brain.set_weights(pickle.loads(weights.data))
    agent.generation = weights.generation

    logger.warning("Initiated agent with weights from generation %s." % agent.generation)
except Exception as e:
    logger.warning("Could not find existing weights, using initial")


# Load all existing experiences
try:
    memory = Memory.select().order_by(Memory.id.desc()).get()
    agent.memory = pickle.loads(memory.data)
except Exception as e:
    print(e)
    logger.warning("Could not find existing experiences")


def train():
    while True:

        if agent.steps > 50:
            agent.replay()
            print("Train generation #%s, memory #%s mem_queue #%s" % (agent.generation, agent.steps, memory_queue.qsize()))

            if agent.generation % GENERATION_SAVE_INTERVAL == 0:
                print("Broadcasting new weights to %s clients." % connected_users)

                # Emit to clients current
                socketio.emit("weight_status", json.dumps({
                    "generation": agent.generation
                }))

                db_w = Weights.create(data=pickle.dumps(agent.brain.get_weights()), generation=agent.generation)
                db_w.save()

                db_mem = Memory.create(data=pickle.dumps(agent.memory), generation=agent.generation)
                db_mem.save()

                while not memory_queue.empty():
                    agent.observe(memory_queue.get())
                    print(memory_queue.qsize())
        else:
            print("Waiting for memories! (%s/%s)" % (agent.steps, 50))
            while not memory_queue.empty():
                agent.observe(memory_queue.get())

        eventlet.sleep(.1)


eventlet.spawn(train)


@app.route('/')
def index():
    return render_template('index.html')


@app.route("/get_weights")
def get_weights():
    pickled = pickle.dumps(agent.brain.get_weights())
    return pickled


@socketio.on('observe', namespace='/')
def observe(data):
    data = json.loads(data)
    s0 = np.array(data[0])
    a = data[1]
    r = data[2]
    t = data[3]
    s1 = np.array(data[4])

    memory_queue.put((s0, a, r, t, s1))



@socketio.on('connect', namespace='/')
def connected():
    global connected_users
    connected_users += 1
    print("Client Connected!")


@socketio.on('disconnect', namespace='/')
def disconnect():
    global connected_users
    connected_users -= 1
    print("Client Disconnected!")


@socketio.on_error()        # Handles the default namespace
def error_handler(e):
    logger.warning(e)


@socketio.on_error_default  # handles all namespaces without an explicit error handler
def default_error_handler(e):
    logger.warning(e)


if __name__ == '__main__':
    socketio.run(app, port=8080)