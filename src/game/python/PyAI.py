
import PyAPIRegistry
import ctypes

from sys import getsizeof
from binascii import hexlify
import numpy as np

class PyAI:
    
    def __init__(self, gameID, playerID):
        self.__ai__ = PyAPIRegistry.hook(gameID, playerID)
        self.desc = [
            "t_id",
            "oil_y",
            "resources",
            "lumber_y",
            "walkable",
            "harvestable",
            "swimable",
            "u_id",
            "u_type_id",
            "u_current_state",
            "gold_carry",
            "lumber_carry",
            "oil_carry",
            "carry_capacity",
            "direction",
            "damage_max",
            "damage_min",
            "damage_piercing",
            "damage_range",
            "health",
            "health_max",
            "military",
            "recallable",
            "sight",
            "structure",
            "player_id",
            "faction"]
    

    def reset(self):
        PyAPIRegistry.reset(self.__ai__)

    def getState(self):
    
        pair = PyAPIRegistry.get_state(self.__ai__)
        buf_ptr = pair[0]
        buf_len = pair[1]
        ROWS =  pair[2]
        COLS = pair[3]
        DEPTH = pair[4]

        memarray = (ctypes.c_int*buf_len).from_address(buf_ptr)

        np_arr = np.array(memarray)
        np_arr = np_arr.reshape((ROWS, COLS, DEPTH)).transpose()
        #np_arr = np.expand_dims(np_arr, axis=0)
        return np_arr

    def doAction(self, action):
        pair = PyAPIRegistry.do_action(self.__ai__, action)
        observation = self.getState()
        reward = pair[0] # Reward
        done = pair[1] # Terminal
        info = pair[2] # Dunno what this is?
        return observation, reward, done, info
        

    def getTime(self):
        pass

    def nextFrame(self):
        pass