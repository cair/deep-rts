import json
import os
from threading import Thread

import tornado.ioloop
import tornado.web
import tornado.websocket
from game import Config

clients = dict()
game = None
pll_worker = None

class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        self.stream.set_nodelay(True)


    def on_message(self, message):
        msg = json.loads(message)

        if msg['type'] == "state":
            self.write_message(json.dumps({
                'type': 'state',
                'data': [state['state'] for state in pll_worker.proc_state.values()]
            }))

    def on_close(self):
        pass


app = tornado.web.Application([
    (r'/websocket', WebSocketHandler),
    (r'/(.*)', tornado.web.StaticFileHandler, {'path': os.path.join(os.path.dirname(__file__), "public_html")}),
])


class WebServer(Thread):

    def __init__(self, _game):
        Thread.__init__(self)
        self.game = _game
        self.pll_worker = _game.parallell_worker

        global pll_worker
        global game
        pll_worker = self.pll_worker
        game = self.game

    def run(self):
        app.listen(Config.WEB_PORT)
        tornado.ioloop.IOLoop.instance().start()
