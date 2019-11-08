import os
import ray
from ray import tune
from ray.rllib.agents.ppo import APPOTrainer, PPOTrainer
from DeepRTS.python.scenario import GoldCollectOnePlayerFifteen
import tensorflow as tf
from util import TensorBoardTool, gpu_count

# https://ray.readthedocs.io/en/latest/tune-usage.html

if __name__ == "__main__":
    ray.init(include_webui=True)

    tb_tool = TensorBoardTool("~/ray_results")
    tb_tool.run()

    tune.run(APPOTrainer,
             name=GoldCollectOnePlayerFifteen.__name__,
             checkpoint_freq=10,
             #resources_per_trial={
             #    "cpu": os.cpu_count(),
             #    "gpu": gpu_count(),
             #    "extra_gpu": 0
             #},
             config=dict(
                 env=GoldCollectOnePlayerFifteen,
                 env_config=dict(
                    rl=dict(
                        updates_per_actions=10,
                        flatten=True,
                    ),
                    engine=dict(),
                    gui=dict()
                 ),
                 eager=False,
                 num_workers=os.cpu_count()-1,
                 num_gpus=gpu_count()
             ))  # "eager": True for eager execution
