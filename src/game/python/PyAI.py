
import PyAPIRegistry
from GameMessage_pb2 import GameMessage

class PyAI:
    
    def __init__(self):
        self.api_id = PyAPIRegistry.hook()

    def getState(self):
        gMessage = GameMessage()
        print();
        return gMessage.ParseFromString(str(PyAPIRegistry.get_state(self.api_id)))

    def getTime(self):
        pass

    def nextFrame(self):
        pass