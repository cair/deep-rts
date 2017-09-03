import Game
x = Game.DeepRTS(4, True)
x.start()
x.createPlayers()
x.initGUI()

x.loop()

