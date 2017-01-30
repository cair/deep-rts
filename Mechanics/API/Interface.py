import threading
import random

from Mechanics import Constants


class Interface(threading.Thread):
    PORT = 20000

    def __init__(self, player):
        """port = Interface.PORT
        Interface.PORT += 1
        address = 'localhost'"""
        threading.Thread.__init__(self)

        if Constants.Config.INTERFACE == "Local":
            print("Hooking AI on player (Local)")
            from AI.AI import AI
            AI(player)


        else:
            print("Remote AI")



    def run(self):
        pass

