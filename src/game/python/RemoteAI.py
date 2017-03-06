import time
import os 


class RemoteAI(CRemoteAI):
    def __init__(self, gameID):
        self.gameID = gameID
		
    def train(self):
        start = time.time()
        next  = start + 1
        counter = 0

        """while True:
            now = time.time()
            l = self.getState(self.gameID)
            counter += 1
            if(now > next):
                next=  time.time() + 1
                print(counter, len(l))
                counter = 0
        """
                

              

