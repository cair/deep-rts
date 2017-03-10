
import PyAPIRegistry
import ctypes
from GameMessage_pb2 import GameMessage
import flatbuffers
from sys import getsizeof
from binascii import hexlify


class PyAI:
    
	def __init__(self):
		self.__ai__ = PyAPIRegistry.hook(0, 0)
	

	def getState(self):
		pair = PyAPIRegistry.get_state(self.__ai__)
		protobuffer = pair[0]
		protobuffer_len = pair[1]

		memarray = (ctypes.c_char*protobuffer_len).from_address(protobuffer)

		gMessage = GameMessage()
		gMessage.ParseFromString(memarray.raw)
		PyAPIRegistry.free(protobuffer)

		return gMessage



	def getTime(self):
		pass

	def nextFrame(self):
		pass