import logging
import sys

import gym
import numpy as np
from tensorboard import default
from tensorboard import program

def get_available_gpus():
    from tensorflow_core.python.client import device_lib
    local_device_protos = device_lib.list_local_devices()
    return [x.name for x in local_device_protos if x.device_type == 'GPU']

def gpu_count():
    return len(get_available_gpus())

class TensorBoardTool:

    def __init__(self, dir_path):
        self.dir_path = dir_path

    def run(self):
        # Remove http messages
        log = logging.getLogger('werkzeug').setLevel(logging.ERROR)
        # Start tensorboard server

        tb = program.TensorBoard(default.get_plugins()) # , default.get_assets_zip_provider()
        tb.configure(argv=[None, '--logdir', self.dir_path])
        url = tb.launch()
        sys.stdout.write('TensorBoard at %s \n' % url)


class LimitedDiscrete(gym.spaces.Space):

    def __init__(self, min_, max_):

        self.min = min_
        self.max = max_
        self.n = max_
        super(LimitedDiscrete, self).__init__((), np.int64)
        self.__class__ = gym.spaces.Discrete

    def sample(self):
        return self.np_random.randint(self.min, self.max)

    def contains(self, x):
        if isinstance(x, int):
            as_int = x
        elif isinstance(x, (np.generic, np.ndarray)) and (x.dtype.kind in np.typecodes['AllInteger'] and x.shape == ()):
            as_int = int(x)
        else:
            return False
        return self.min <= as_int < self.max

    def __repr__(self):
        return "Discrete(%d-%d)" % (self.min, self.max)

    def __eq__(self, other):
        return isinstance(other, LimitedDiscrete) and self.min == other.min and self.max == other.max
