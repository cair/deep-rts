from os import listdir, path
from os.path import isfile, join
from tqdm import tqdm
from games.Score.ScoreLog import ScoreLog
from games.Score.ScorePlayer import ScorePlayer
from games.Score.ScoreItem import ScoreItem
import sys
import json
import io
import gzip
import os
from multiprocessing import Pool, TimeoutError
import multiprocessing

dir_path = os.path.dirname(os.path.realpath(__file__))
try:
  os.makedirs(join(dir_path, "games_compressed_json"))
except:
  pass
output_dir = join(dir_path, "games_compressed_json")


def compress_json(json_str):
    data = bytes(json_str, "utf-8")
    s_out = gzip.compress(data)
    return s_out
    
def decompress_json(gzip_data):
  json_str = gzip.decompress(gzip_data)
  return str(json_str, "utf-8")

def load_buffer(filename):
    buf = open(filename, 'rb').read()
    buf = bytearray(buf)
    log = ScoreLog.GetRootAsScoreLog(buf, 0)
    return log
    
def find_files(search_path="./games/", ext=".flat"):
    onlyfiles = [path.abspath(join(search_path, f)) for f in listdir(search_path) if isfile(join(search_path, f)) and f.endswith(ext)]
    return onlyfiles
    
    
def parse(filename):
    data = {
      "game_num": None,
      "items":[]
    }
    
    log_item = load_buffer(filename)
    
    data["game_num"] = log_item.GameNum()
    
    for i in range(log_item.ItemsLength()):
      # ScorePlayer
      player_item = log_item.Items(i)
      player_id = player_item.PlayerID()
      
      # Create new data container for ScorePlayer
      player_data = {"player_id": player_id, "items": []}
      data["items"].append(player_data)
      
      for tick in range(player_item.ItemsLength()):
        # ScoreItem
        score_item = player_item.Items(tick)
        try:
            apm = score_item.Apm()
        except:
            os.remove(filename)
            return
        score = score_item.Score()
        action_stats = [score_item.ActionStats(i) for i in range(score_item.ActionStatsLength())]
        
        player_data["items"].append({
          "apm": apm,
          "score": score,
          "tick": tick,
          "action_stats": action_stats
        })
        
    json_str = json.dumps(data)
    compressed = compress_json(json_str)
    #decompressed = decompress_json(compressed)
    #loaded_back = json.loads(decompressed)
    #print(sys.getsizeof(json_str), " => ", sys.getsizeof(compressed), " => ", sys.getsizeof(decompressed))
    new_file_name = os.path.splitext(filename)[0]
    new_file_name += ".json"
    new_file_name = os.path.basename(new_file_name)

    with open(join(output_dir, new_file_name), "wb+") as file:
        file.write(compressed)
        
    os.remove(filename)
        
    """with open(join(output_dir, new_file_name), "rb") as file:
        loaded_back = json.loads(decompress_json(file.read()))
        print(loaded_back)"""
            
files = find_files()
n_files = len(files)

# Create a process pool
pool = Pool(processes=multiprocessing.cpu_count())

# Process data
for i, data in tqdm(enumerate(pool.imap_unordered(parse, files)), total=n_files, desc="Files Converted"):
  pass
