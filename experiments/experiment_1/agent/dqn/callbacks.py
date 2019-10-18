from tensorflow.python.keras.callbacks import Callback as KerasCallback
import time


class Callback(KerasCallback):
    """
    on_epoch_begin: called at the beginning of every epoch.
    on_epoch_end: called at the end of every epoch.
    on_batch_begin: called at the beginning of every batch.
    on_batch_end: called at the end of every batch.
    on_train_begin: called at the beginning of model training.
    on_train_end: called at the end of model training.

    """
    def __init__(self):
        super(Callback, self).__init__()
        self.duration = 0
        self._start = None
        self.epoch = 0

    def on_train_begin(self,  logs={}):
        self._start = time.time()

    def on_train_end(self, logs={}):
        self.duration = time.time() - self._start
        self.epoch += 1


class EpochCallback(KerasCallback):
    """
    on_epoch_begin: called at the beginning of every epoch.
    on_epoch_end: called at the end of every epoch.
    on_batch_begin: called at the beginning of every batch.
    on_batch_end: called at the end of every batch.
    on_train_begin: called at the beginning of model training.
    on_train_end: called at the end of model training.

    """
    def __init__(self):
        super(EpochCallback, self).__init__()
        self.duration = 0
        self._start = None
        self.epoch = 0

    def on_epoch_begin(self, epoch, logs={}):
        self._start = time.time()

    def on_epoch_end(self, epoch, logs={}):
        self.duration = time.time() - self._start
        self.epoch += 1


class ModelIntervalCheckpoint(Callback):
    def __init__(self, filepath, interval, verbose=0):
        super(ModelIntervalCheckpoint, self).__init__()
        self.filepath = filepath
        self.interval = interval
        self.verbose = verbose

    def on_train_end(self, logs={}):
        super().on_train_end(logs)
        if self.epoch % self.interval != 0:
            # Nothing to do.
            return

        filepath = self.filepath.format(step=self.epoch, **logs)
        if self.verbose > 0:
            print('Epoch %s: Saving model to %s' % (self.epoch, filepath))

        self.model.save_weights(filepath, overwrite=True)


class LossCallback(EpochCallback):
    def __init__(self, interval):
        super(LossCallback, self).__init__()
        self.interval = interval

    def on_epoch_end(self, epoch, logs={}):
        super().on_train_end(logs)
        self.epoch += 1

        if self.epoch % self.interval == 0:
            keys = logs.keys()
            if "loss" in keys:
                loss = logs["loss"]
            else:
                loss = "N/A"
            if "acc" in keys:
                acc = logs["acc"]
            else:
                acc = "N/A"

            print("Epoch %s: Loss: %s, Accuracy: %s" % (self.epoch, loss, acc))



class TargetModelUpdateCallback(Callback):
    def __init__(self, target, interval, verbose=0):
        super(TargetModelUpdateCallback, self).__init__()
        self.target_model = target
        self.interval = interval
        self.verbose = verbose
        self.last_update = 0

    def on_train_end(self, logs={}):
        super().on_train_end(logs)

        if self.epoch % self.interval == 0:
            self.target_model.set_weights(self.model.get_weights())

            if self.verbose > 0:
                print("Epoch %s: Updating Target model from epoch %s to %s" % (self.epoch, self.last_update, self.epoch))
            self.last_update = self.epoch
