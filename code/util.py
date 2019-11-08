import logging
import sys

from tensorboard import default
from tensorboard import program
from tensorflow_core.python.client import device_lib


def get_available_gpus():
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
