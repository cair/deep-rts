from socketIO_client import SocketIO



def update_weights(message):
    print(message)

socketIO = SocketIO('localhost', 8080)
socketIO.on('update_weights', update_weights)

socketIO.wait(1)