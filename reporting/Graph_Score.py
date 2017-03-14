import flatbuffers
from Score.ScoreLog import ScoreLog
from Score.ScorePlayer import ScorePlayer
from Score.ScoreItem import ScoreItem
import plotly
import plotly.graph_objs as go
from os import listdir, path
from os.path import isfile, join
from tqdm import tqdm
import numpy as np
import time
from multiprocessing import Pool, TimeoutError
import multiprocessing

plot_individual = False
plot_average = True
list_init_size = 5000

# This is a nested array (1 sub array per player) with averages per player
average_plots = [
[[0] * list_init_size,[0] * list_init_size], # Player 0
[[0] * list_init_size,[0] * list_init_size], # Player 1
[[0] * list_init_size,[0] * list_init_size], # Player 2
[[0] * list_init_size,[0] * list_init_size], # Player 3
# It is automatically expanded if needed
]

player_colors = [
    'rgb(255, 0, 0)'
    'rgb(0, 0, 255)',
    'rgb(0, 255, 0)',
    'rgb(0, 255, 255)',
]


def find_files(search_path=".", ext=".flat"):
    onlyfiles = [path.abspath(f) for f in listdir(search_path) if isfile(join(search_path, f)) and f.endswith(ext)]
    return onlyfiles
    
def load_buffer(filename):
    buf = open(filename, 'rb').read()
    buf = bytearray(buf)
    log = ScoreLog.GetRootAsScoreLog(buf, 0)
    return log
    
def parse(filename):
    #print(filename)

    ret_plots = []
    log_item = load_buffer(filename)
    gameNum = log_item.GameNum()
    for i in range(log_item.ItemsLength()):
        player_item = log_item.Items(i)
        playerID = player_item.PlayerID()
        

        # init Data point vectors for plotly
        data_x = [0] * list_init_size
        data_y = [0] * list_init_size
        
        for tick in range(player_item.ItemsLength()):
            score_item = player_item.Items(tick)
            apm = score_item.Apm()
            score = score_item.Score()
            #action_stats = [score_item.ActionStats(x) for x in range(score_item.ActionStatsLength())]
        
            # Add tick as X axis
            data_x[tick] = tick
            
            # Add score as Y axis
            data_y[tick] = score

            # Add tick as X axis in average plot for player

            average_plots[i][0][tick] += tick

            # Add score as Y axis in average plot for player
            average_plots[i][1][tick] += score

        # Add plot to plot list
        ret_plots.append((data_x, data_y, playerID))  #
    return ret_plots
            
        
if __name__ == "__main__":
    # Plot Data members
    plots = []
    average_plots = {}  # Collection of average plotting per player

    # File Data members
    files = find_files()
    num_files = len(files)
    num_processed = 0

    # Create a process pool
    pool = Pool(processes=multiprocessing.cpu_count() - 4)

    # Process data
    for i, data in tqdm(enumerate(pool.imap_unordered(parse, files)), total=len(files), desc="Files processed"):

        for game in data:
            data_x, data_y, playerID = game

            if plot_individual:
                plots.append(go.Scatter(x=data_x, y=data_y, name="Player %s" % playerID, line=dict(color=player_colors[playerID], width=4)))

            if plot_average:
                if playerID not in average_plots:
                    average_plots[playerID] = [[0] * len(data_x), [0] * len(data_y)]

                for point in range(len(data_x)):
                    average_plots[playerID][0][point] += data_x[point]
                    average_plots[playerID][1][point] += data_y[point]

        num_processed += 1

    if plot_average:
        for playerID, value in average_plots.items():

         # Remove trailing zeros
            while value[0][-1] == 0:
                value[0].pop()
                value[1].pop()

            avg_y = np.trim_zeros(np.array(value[1]) / num_processed, 'b')
            avg_x = np.array([i for i in range(len(avg_y))])
            plots.append(go.Scatter(x=avg_x,  y=avg_y, name = "Player %s" % playerID))




    layout = go.Layout(
        title='DeepRTS AI Results',
        xaxis=dict(
            title='Game Frame',
            titlefont=dict(
                family='Courier New, monospace',
                size=24,
                color='#7f7f7f'
            )
        ),
        yaxis=dict(
            title='Player Score',
            titlefont=dict(
                family='Courier New, monospace',
                size=24,
                color='#7f7f7f'
            )
        )
    )
    fig = go.Figure(data=plots, layout=layout)
    plotly.offline.plot(fig, filename="deeprts_game_%s.png" % (int(time.time())),  image='png')

    


