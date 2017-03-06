import time
import os 
dir_path = os.path.dirname(os.path.realpath(__file__))
print(dir_path)
from GameMessage_pb2 import GameMessage

class RemoteAI(CRemoteAI):
    def __init__(self, gameID):
        self.gameID = gameID
		
    def train(self):
        start = time.time()
        next  = start + 1
        counter = 0

        while True:
            now = time.time()
            l = self.getState(self.gameID)
            counter += 1
            if(now > next):
                next=  time.time() + 1
                print(counter, len(l))
                counter = 0
                

              



       

        #GameMessage.ParseFromString(l)
