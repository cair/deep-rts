import asyncio
import websockets
import logging
log = logging.getLogger('root')


class RemoteAI:
    PORT = 20000

    def __init__(self, player, event):
        self.port = RemoteAI.PORT
        self.host = "0.0.0.0"
        RemoteAI.PORT += 1
        self.Event = event
        self.player = player
        self.connected = set()
        self.loop = None

        self.server = websockets.serve(self.on_receive, self.host, self.port)

        """
        Hook events
        """
        event.c_event = self.on_event

    def init(self):
        self.loop = asyncio.new_event_loop()
        loop = self.loop
        asyncio.set_event_loop(loop)
        loop.run_until_complete(self.server)
        loop.run_forever()

    async def on_receive(self, websocket, path):
        self.connected.add(websocket)
        try:
            await websocket.recv()
        except websockets.exceptions.ConnectionClosed:
            pass
        finally:
            self.connected.remove(websocket)
            log.debug("[Remote AI] Disconnected client.")

    def on_event(self, event_type, data):
        for client in self.connected.copy():
            coro = client.send("%s->%s" % (event_type, data))
            future = asyncio.run_coroutine_threadsafe(coro, self.loop)
