
import sys
import time

import RemoteAI


remAI = RemoteAI.RemoteAI()
remAI.connect(5000)

print(remAI.send({"type": "getState"}))
