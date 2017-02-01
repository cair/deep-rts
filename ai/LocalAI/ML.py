
def d_kills(old, new):
    return (new - old) * 100

def d_units(old, new):
    return (old - new) * 50 # - old_enemy - new enemy

def d_resource(old, new):
    return (old - new) / 10

def reward(previous_state_scores, current_state_scores):
    return d_kills(1,1) + d_resource(1,1) + d_units(1,1)
