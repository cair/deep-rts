
import sys
from PyAI import PyAI
from Algorithms.DQN.DQN import DQN
print("Running PyAI using Python %s" % (sys.version))
PyAPIRegistry.loaded() # Tell C++ that we are done with dependencies

pyai = PyAI(0, 0)  # Create new AI hook with gameID = 0 playerID= 0
dqn = DQN(pyai)  # Create new DQN using the pyai hook