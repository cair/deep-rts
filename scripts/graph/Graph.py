from tqdm import tqdm
import json
import os
from multiprocessing import Pool, TimeoutError
import multiprocessing
import numpy as np
import plotly
import plotly.graph_objs as go
import time
import gzip
from os import listdir, path
from os.path import isfile, join


if isfile(os.path.expanduser("~") + "/.plotly/.credentials"):
    print("Found ~/.plotly/.credentials... Assuming its OK")
else:
    print("No plotly credentials found. Please enter Username and API (No validation done)")
    usr = input("Enter plotly username: ")
    api = input("Enter plotly api key: ")
    plotly.tools.set_credentials_file(username=usr, api_key=api)


n_players = 4
max_files = None

def compress_json(json_str):
    data = bytes(json_str, "utf-8")
    s_out = gzip.compress(data)
    return s_out

def find_files(search_path="../../c++/build/games/", ext=".json"):
    onlyfiles = [path.abspath(join(search_path, f)) for f in listdir(search_path) if isfile(join(search_path, f)) and f.endswith(ext)]
    return onlyfiles

def decompress_json(gzip_data):
    json_str = gzip.decompress(gzip_data)
    return str(json_str, "utf-8")


def load_json(filename):
    with open(filename, "r") as file:
        data = file.read()
        try:
            json_data = json.loads(data)
        except:
            json_data = json.loads(decompress_json(data))

    return json_data



def parse(filename):
    return_data = []  # Create a return container
    data = load_json(filename)  # Load JSON from file
    #game_num = data["game_num"]  # Retrieve game number
    
    for player_item in data["players"]:
        player_name = player_item["name"]
        data_x = []
        data_y = []
        
        for tick, score_item in enumerate(player_item["ticks"]):
            apm = score_item["apm"]
            score = score_item["score"]
            #action_stats = score_item["action_stats"]
            
            data_x.append(tick)
            data_y.append(score)

        return_data.append((player_name, np.array(data_x), np.array(data_y)))
    return return_data

json_files = find_files()[:max_files] if max_files else find_files()

score_data = {}

# Create a process pool
pool = Pool(processes=multiprocessing.cpu_count() - 4)

# Process data
c = 0
for i, data in tqdm(enumerate(pool.imap_unordered(parse, json_files)), total=len(json_files), desc="Files processed"):
    c += 1

    for graph_data in data:
        player_name, graph_x, graph_y = graph_data

        if player_name not in score_data:
            score_data[player_name] = []

        for y_i, y in enumerate(graph_y):
            try:
              score_data[player_name][y_i] += y
            except:
              score_data[player_name].append(y)
              
 
plots = []
# Finish of average
for player_name, graph_y in score_data.items():
    score_y = np.array(graph_y)

    score_data[player_name] = score_y / c
    plots.append(go.Scatter(y=score_data[player_name], name=player_name))

layout = go.Layout(
        title='DeepRTS AI Results (%s Games)' % c,
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
plot_name = "DeepRTS_ScoreGraph_%s.html" % (int(time.time()))
fig = go.Figure(data=plots, layout=layout)
#plotly.offline.plot(fig, filename="DeepRTS_ScoreGraph_%s.html" % (int(time.time())),  image='png')
plotly.plotly.plot(fig, filename=plot_name,  image='png')
               

# Lastly prompt for archiving
ans = "DontKnow"
while ans != "Yes" and ans != "No":
    ans = input("Do you want to archive current game_log-set (Yes/No)")
    print(ans)

if ans == "Yes":
    # Archive
    import tarfile
    archive_name = plot_name + ".tar.xz"
    tar = tarfile.open(archive_name, "w:xz")
    for name in tqdm(json_files):
        tar.add(name)
    tar.close()

    # Saved
    print("Saved archive to %s" % archive_name)
    print("Moving all json files to /tmp/")
    old_dir = "/tmp/%s" % plot_name + ".d"
    os.makedirs(old_dir)
    for file in tqdm(json_files):
        os.rename(file, join(old_dir, os.path.basename(file)))
    print("Moved all files to %s" % old_dir)




