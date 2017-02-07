import asyncio
import logging
import time

import websockets

from ai.RemoteAI import Config

log = logging.getLogger('root')


class Benchmarker:

    def __init__(self):
        self.ticks = 0
        self.next_second = time.time() + 1.0


class RemoteClient:

    def __init__(self, on_event=None):
        self.on_event = on_event

        self.benchmark = Benchmarker()


        loop = asyncio.get_event_loop()
        loop.run_until_complete(self.client_connect())
        loop.run_forever()

    async def client_connect(self):
        log.info('Connecting to ws://%s:%s' % (Config.HOST, Config.PORT))
        websocket = await websockets.connect('ws://%s:%s' % (Config.HOST, Config.PORT))
        log.info('Connected!')

        # On event
        asyncio.ensure_future(self.on_message(websocket))

    async def on_message(self, websocket):
        while True:
            await asyncio.sleep(0)

            try:
                data = await websocket.recv()



                # Benchmarking
                t = time.time()
                self.benchmark.ticks += 1
                if t >= self.benchmark.next_second:
                    print(self.benchmark.ticks)
                    self.benchmark.next_second = t + 1.0
                    self.benchmark.ticks = 0




            except websockets.ConnectionClosed as cc:
                print('Connection closed')
                exit(0)

            except Exception as e:
                print('Something happened', e)



