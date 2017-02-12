"""
This scripts visualize the score graph scaling
"""
import plotly
import plotly.graph_objs as go
from game.WarC2 import Game
from game.logic.Player.Player import Player
from game.logic.Unit.Peasant import Peasant
from game.logic.Unit.Footman import Footman
from game.logic.Unit.TownHall import TownHall
from game.logic.Unit.Barracks import Barracks
from game.logic.Unit.Farm import Farm
from reports.Mocks import MockGame
import numpy as np

"""score_types = [
    'score_unit',
    'score_structure',
    'score_resources',
    'score_military',
    'score_defence',
    'score_total'
]"""

graph_items = {
    'score_peasant': [['score_unit'], Peasant],
    'score_footman': [['score_unit', 'score_military'], Footman],
    'score_town_hall': [['score_structure'], TownHall],
    'score_farm': [['score_structure'], Farm],
    'score_barracks': [['score_structure'], Barracks]
}


N = 30
x = [x for x in range(N)]
traces = []

for key, item in graph_items.items():
    ys = {
        x: [] for x in item[0]
        }
    g = Game()                                 # Create a game instance
    g.pause()                                           # Ensure that it does not run
    p = Player(g, 0)                                    # Create a player
    p.units = []                                        # Remove all units

    for i in range(N):
        unit = item[1](p)
        unit.add_to_game()

        for score_type in item[0]:

            f = getattr(p, score_type)()
            ys[score_type].append(f)

    for score_type in item[0]:
        trace0 = go.Scatter(
            x=x,
            y=ys[score_type],
            mode='lines',
            name=key.split("_")[1] + "_" + score_type
        )
        traces.append(trace0)



# Plot and embed in ipython notebook!
plotly.offline.plot(traces, filename='basic-line')
plotly.offline.plot(traces, image='png')

# or plot with: plot_url = py.plot(data, filename='basic-line')