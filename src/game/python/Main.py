
import sys
import time

import random
import numpy as np
from collections import deque

import json
from keras import initializations
from keras.initializations import normal, identity
from keras.models import model_from_json
from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation, Flatten
from keras.layers.convolutional import Convolution2D, MaxPooling2D
from keras.optimizers import SGD , Adam
from PyAI import PyAI



def bootstrap():
    print("called!")


pyai = PyAI()

now = time.time()
next = time.time() + 1
counter = 0
while True:
    now = time.time()
    pyai.getState()
    counter += 1

    if(now > next):
        next = time.time() + 1
        print("Serialized: " + str(counter) + " times")
        counter = 0



#print(sys.argv)

print("Hehehe")
