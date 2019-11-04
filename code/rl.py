from ray import tune
from ray.rllib.agents.ppo import PPOTrainer
from DeepRTS.python.scenario import GoldCollectOnePlayerFifteen
if __name__ == "__main__":

    tune.run(PPOTrainer, config={"env": GoldCollectOnePlayerFifteen})  # "eager": True for eager execution
