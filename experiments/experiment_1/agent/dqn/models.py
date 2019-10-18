from tensorflow.python.keras import backend as K
from tensorflow.python.keras import Input
from tensorflow.python.keras.layers import Activation, ConvLSTM2D, Reshape, Lambda
from tensorflow.python.keras.models import Sequential, Model
from tensorflow.python.keras.optimizers import Adam
from tensorflow.python.layers.convolutional import Conv2D
from tensorflow.python.layers.core import Flatten, Dense

from .capsules.layers import PrimaryCap, CapsuleLayer, Length


def capsnet(state_size, action_size):
    routings = 3

    x = Input(shape=state_size)

    # Layer 1: Just a conventional Conv2D layer
    conv1 = Conv2D(64, (4, 4), strides=(2, 2), activation=Activation("relu"))(x)
    #conv2 = Conv2D(64, (3, 3), strides=(2, 2), activation=Activation("relu"))(conv1)

    # Layer 2: Conv2D layer with `squash` activation, then reshape to [None, num_capsule, dim_capsule]
    primarycaps = PrimaryCap(conv1, dim_capsule=8, n_channels=32, kernel_size=9, strides=2, padding='valid')

    # Layer 3: Capsule layer. Routing algorithm works here.
    digitcaps = CapsuleLayer(num_capsule=action_size, dim_capsule=16, routings=routings,
                             name='digitcaps')(primarycaps)

    # Layer 4: This is an auxiliary layer to replace each capsule with its length. Just to match the true label's shape.
    # If using tensorflow, this will not be necessary. :)
    out_caps = Length(name='capsnet')(digitcaps)
    model = Model(inputs=[x], outputs=[out_caps])
    return model


def cnn_dualing(observation_space, action_space, lr):
    # Neural Net for Deep-Q learning Model

    # Image input
    input_layer = Input(shape=observation_space[1:], name='image_input')
    x = Conv2D(64, (8, 8), strides=(1, 1), activation='relu', data_format="channels_first")(
        input_layer)
    x = Conv2D(64, (4, 4), strides=(1, 1), activation='relu', data_format="channels_first")(x)
    x = Conv2D(64, (3, 3), strides=(1, 1), activation='relu', data_format="channels_first")(x)
    x = Reshape((int(x.shape[1]), int(x.shape[2]), int(x.shape[3])))(x)
    x = Flatten()(x)

    # Value Stream
    vs = Dense(512, activation="relu", kernel_initializer='uniform')(x)
    vs = Dense(1, kernel_initializer='uniform')(vs)

    # Advantage Stream
    ad = Dense(512, activation="relu", kernel_initializer='uniform')(x)
    ad = Dense(action_space, kernel_initializer='uniform', activation="linear")(ad)

    policy = Lambda(lambda w: w[0] - K.mean(w[0]) + w[1])([vs, ad])

    model = Model(inputs=[input_layer], outputs=[policy])
    optimizer = Adam(lr=lr)
    model.compile(optimizer=optimizer, loss="mse")

    return model


def cnn(state_size, action_size):
    model = Sequential()
    model.add(Conv2D(32, (8, 8), strides=(4, 4), activation=Activation("relu"), input_shape=state_size))
    model.add(Conv2D(32, (4, 4), strides=(2, 2), activation=Activation("relu")))
    model.add(Conv2D(32, (3, 3), strides=(1, 1), activation=Activation("relu")))
    model.add(Flatten())
    model.add(Dense(512, activation=Activation("relu")))
    model.add(Dense(action_size, activation=Activation("linear")))
    model.summary()
    return model


def cnn_rnn(state_size, action_size):

    model = Sequential()
    model.add(ConvLSTM2D(64, (1, 1), return_sequences=True, strides=(1, 1), activation=Activation("relu"), input_shape=(4, ) + state_size))
    model.add(ConvLSTM2D(64, (1, 1), return_sequences=True, strides=(1, 1), activation=Activation("relu")))
    model.add(ConvLSTM2D(64, (1, 1), return_sequences=False, strides=(1, 1), activation=Activation("relu")))
    model.add(Flatten())
    model.add(Dense(1024, activation=Activation("relu")))
    model.add(Dense(action_size, activation=Activation("linear")))
    return model