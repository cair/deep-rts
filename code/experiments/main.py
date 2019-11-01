import argparse
import os
import importlib
import config

if __name__ == "__main__":
    current_directory = os.path.dirname(os.path.abspath(__file__))
    parser = argparse.ArgumentParser(description="DeepRTS-experiments")
    parser.add_argument("--experiment", metavar="E", type=int, help="Define which experiment to run")
    parser.add_argument("--gpu", metavar="G", type=int, help="To use GPU or CPU", default=1)
    parser.add_argument("--viz", metavar="V", type=int, help="Visualize via web", default=config.visualize)
    args = parser.parse_args()

    experiments = [name for name in os.listdir(current_directory) if os.path.isdir(os.path.join(current_directory, name)) and "experiment_" in name]
    selected_experiment = "experiment_%s" % (args.experiment)
    if selected_experiment in experiments:
        experiment_path = os.path.join(current_directory, selected_experiment)
        experiment_runner_path = os.path.join(experiment_path, "main.py")

        if os.path.exists(experiment_runner_path) and os.path.isfile(experiment_runner_path):
            print("Running %s" % experiment_runner_path)
            importlib.import_module(selected_experiment + ".main")
        else:
            exit("Could not run %s" % experiment_runner_path)

    else:
        exit("Could not find %s" % (selected_experiment))
