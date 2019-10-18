from tensorflow.python.keras import backend as K


def huber_loss_1(target, prediction):
    # sqrt(1+error^2)-1
    error = prediction - target
    return K.mean(K.sqrt(1 + K.square(error)) - 1, axis=-1)
