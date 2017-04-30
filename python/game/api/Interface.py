import logging
import threading
from game import Config
from game.api.LocalAI import LocalAI
from game.api.RemoteAI import RemoteAI

log = logging.getLogger('root')


class Interface(threading.Thread):

    def __init__(self, player, event):
        threading.Thread.__init__(self)

        if Config.INTERFACE == Config.AI_LOCAL:
            log.info("Hooking AI on player (Local)")
            self.ai = LocalAI(player, event)
        else:
            log.info("Hooking AI on player (Remote)")
            self.ai = RemoteAI(player, event)

    def run(self):
        self.ai.init()

