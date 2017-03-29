import copy
import os
import time


from game import Config
from game.graphics.GUI import GUI
from game.player.Player import Player
from game.state.StateManager import StateManager
from sfml.sf import Clock

from game.environment.Tilemap import Tilemap


class Game:
    def __init__(self):
        self.map = Tilemap("contested-4v4.json")
        self.state_manager = StateManager()

        self.units = []
        self.players = []

        self.update_next = None
        self.render_next = None
        self.apm_next = None
        self.stats_next = None

        self.update_interval = 1000000 / 600
        self.render_interval = 1000000 / 60
        self.apm_interval = 1000000 / 600

        self.current_ups = 0
        self.current_fps = 0

        self.update_delta = 0
        self.render_delta = 0

        self.ticks = 0

        self.doDisplay = True
        self.doCaptionConsole = True
        self.doCaptionWindow = False


        # Create GUI
        self.gui = GUI(self)

        self.clock = Clock()

    def add_player(self):
        player = Player(self, len(self.players))
        self.players.append(player)

        self.spawn_player(player)

        return player

    def spawn_player(self, player):

        spawn_point_index = self.map.spawn_tiles[player.id]
        spawn_tile = self.map.tiles[spawn_point_index]


        builder = player.spawn(spawn_tile)

        if Config.MECHANICS_TOWN_HALL_ON_START:
            builder.build(0)


    def loop(self):
        now = self.clock.elapsed_time.microseconds
        self.update_next = now
        self.render_next = now
        self.apm_next = now
        self.stats_next = now

        # Do 1 iteration on loop anyways
        self.update()

        # Continue looping if game should run
        while Config.IS_RUNNING:
            self.update()

    def update(self):
        now = self.clock.elapsed_time.microseconds

        if now >= self.update_next:

            # Update all units
            for unit in self.units:
                if unit.removed:
                    continue

                unit.update()

            # Update players
            for player in self.players:
                player.update()

            # TODO score logging

            self.update_next += self.update_interval
            self.update_delta += 1
            self.ticks += 1

        if now >= self.apm_next:
            # Update algorithms for playe
            for player in self.players:
                if not player.algorithm:
                    continue

                player.algorithm.update()

            self.apm_next += self.apm_interval

        if self.doDisplay and now >= self.render_next:
            self.gui.update()
            self.gui.render()

            self.render_next += self.render_interval
            self.render_delta += 1

        if now >= self.stats_next:

            if self.doDisplay and self.doCaptionWindow:
                self.gui.caption()

            if self.doCaptionConsole:
                print("[FPS=%s, UPS=%s]" % (self.current_fps, self.current_ups))

            if True: # TODO add option
                for player in self.players:
                    player.apm = (player.apm + (player.apm_counter * 60)) / 2
                    player.apm_counter = 0

            self.current_fps = self.render_delta
            self.current_ups = self.update_delta
            self.render_delta = 0
            self.update_delta = 0
            self.stats_next += 1000000 # 1 second


