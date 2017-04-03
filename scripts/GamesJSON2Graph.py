import Games2JSON
from tqdm import tqdm
import json
from multiprocessing import Pool, TimeoutError
import multiprocessing
import numpy as np
import plotly
import plotly.graph_objs as go
import time

size = 12000
n_players = 4
max_files = None

def parse(filename):
    retData = []


    with open(filename, "rb") as file:
            data = json.loads(Games2JSON.decompress_json(file.read()))

    game_num = data["game_num"]
    
    for player_item in data["items"]:
        player_id = player_item["player_id"]
        data_x = []
        data_y = []
        
        for tick, score_item in enumerate(player_item["items"]):
            apm = score_item["apm"]
            score = score_item["score"]
            action_stats = score_item["action_stats"]
            
            data_x.append(tick)
            data_y.append(score)
            
            
        retData.append((np.array(data_x), np.array(data_y)))
    return retData



json_files = Games2JSON.find_files("./games_compressed_json", ".json")
if max_files:
    json_files = json_files[:max_files]


# Create average dataset
score_data = []
for i in range(n_players):
    score_data.append([[0 for i in range(size)],[0 for i in range(size)]])

# Create a process pool
pool = Pool(processes=multiprocessing.cpu_count() - 4)

# Process data
c = 0
for i, data in tqdm(enumerate(pool.imap_unordered(parse, json_files)), total=len(json_files), desc="Files processed"):
    c += 1

    for player_id, graph_data in enumerate(data):
        graph_y = graph_data[1]
         
        for y_i, y in enumerate(graph_y):
            try:
              score_data[player_id][1][y_i] += y
            except:
              score_data[player_id][1].append(y)
              
 
plots = []
# Finish of average
for player_id, player_data in enumerate(score_data):
  score_x = player_data[0]
  score_y = np.array(player_data[1])
  
  player_data[1] = score_y / c
  plots.append(go.Scatter(  y=player_data[1], name = "Player %s" % player_id))

  
  

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

               






