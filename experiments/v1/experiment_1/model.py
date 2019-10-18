import tensorflow as tf
from tensorflow.contrib.keras import backend as K
from tensorflow.contrib.keras.python.keras.layers import Conv2D, Flatten, Dense
from tensorflow.contrib.keras.python.keras.models import Sequential
from tensorflow.contrib.keras.python.keras.optimizers import RMSprop
import numpy as np




def huber_loss(y_true, y_pred):
    err = y_true - y_pred

    cond = K.abs(err) < Brain.HUBER_LOSS_DELTA
    L2 = 0.5 * K.square(err)
    L1 = Brain.HUBER_LOSS_DELTA * (K.abs(err) - 0.5 * Brain.HUBER_LOSS_DELTA)

    loss = tf.where(cond, L2, L1)   # Keras does not cover where function in tensorflow :-(

    return K.mean(loss)


class Brain:
    HUBER_LOSS_DELTA = 2.0

    def __init__(self,
                 state_shape,
                 ACTION_COUNT=16,
                 LEARNING_RATE=0.000025
                 ):
        self.LEARNING_RATE = LEARNING_RATE
        self.ACTION_COUNT = ACTION_COUNT

        self.state_shape = state_shape
        self.graph = tf.get_default_graph()
        self.model = self._create_model()
        self.model_ = self._create_model()  # target network
        self.loss = 0

    def _create_model(self):
        with self.graph.as_default():
            model = Sequential()

            #model.add(Conv2D(32, (8, 8), strides=(2, 2), activation='relu', input_shape=self.state_shape, data_format='channels_first'))
            #model.add(Conv2D(64, (4, 4), strides=(2, 2), activation='relu'))
            #model.add(Conv2D(64, (3, 3), activation='relu'))
            model.add(Dense(units=512, activation='relu', input_shape=self.state_shape))
            model.add(Flatten())
            model.add(Dense(units=512, activation='relu'))

            model.add(Dense(units=self.ACTION_COUNT, activation='linear'))

            opt = RMSprop(lr=self.LEARNING_RATE)
            model.compile(loss=huber_loss, optimizer=opt)

            return model

    def train(self, x, y, epochs=1, verbose=0):
        with self.graph.as_default():
            history = self.model.fit(x, y, batch_size=32, epochs=epochs, verbose=verbose)
            current_loss = history.history["loss"][0]
            self.loss = (self.loss + current_loss) / 2

    def predict(self, s, target=False):
        with self.graph.as_default():
            if target:
                return self.model_.predict(s)
            else:
                return self.model.predict(s)

    def predict_one(self, s, target=False):
        return self.predict(np.array([s]), target).flatten()

    def update_target_model(self):
        with self.graph.as_default():
            self.model_.set_weights(self.model.get_weights())

    def get_weights(self):
        with self.graph.as_default():
            return self.model_.get_weights()

    def set_weights(self, w):
        with self.graph.as_default():
            self.model_.set_weights(w)
