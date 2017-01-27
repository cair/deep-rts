from Event import Event
from GameClock import GameClock
from Mechanics.Map import Map
from Mechanics.Player import Player
from GUI.GUI import GUI
from GUI.NoGUI import NoGUI
from Mechanics.Constants import Config


class Game(object):

    def __init__(self, map_name="simple", players=2):

        # Create Map
        self.map = Map(self, map_name)

        # Unit Map
        self.units = {}

        # Create Players
        self.players = [Player(self, "Player %s" % x) for x in range(players)]
        [player.spawn() for player in self.players]

        # Create GUI
        self.gui = NoGUI(self) if not Config.HAS_GUI else GUI(self, self.players[0])

        # Create game clock
        self.clock = GameClock()
        self.clock.shedule(self.gui.caption, 1.0)
        self.clock.update(self.process, 90000)  # 16
        self.clock.render(self.render, 60) # 60

        self.loop()

    def loop(self):
        while Config.IS_RUNNING:
            self.clock.tick()

    def process(self, tick, frame):

        for p in self.players:
            p.process(tick)

        Event.notify(Event.New_State, frame)

    def render(self, tick):
        self.gui.render(tick)

