
# Run setup.py install in sources/python to build source files.

from DeepRTS.python import scenario
from DeepRTS.Engine import Random

import Scenarios
import Agents

import torch
import imageio

import os
from datetime import datetime

import pygame

# to print out action name do:
action_names = {

    1: "Previous Unit",
    2: "Next Unit",

    3: "Move Left",
    4: "Move Right",
    5: "Move Up",
    6: "Move Down",
    7: "Move Up Left",
    8: "Move Up Right",
    9: "Move Down Left",
    10: "Move Down Right",

    11: "Attack",
    12: "Harvest",

    13: "Build 0",
    14: "Build 1",
    15: "Build 2",

    16: "No Action"
}

if __name__ == "__main__":

    # Generating the output folders

    results_folder = "Results"

    now = datetime.now()
    now_string = now.strftime("%d-%m-%Y %H-%M-%S")
    directory = "Results " + now_string


    results_path = os.path.join(os.getcwd(), results_folder)
    results_path = os.path.join(results_path, directory)
    a_path = os.path.join(results_path, "Agent A")
    b_path = os.path.join(results_path, "Agent B")
    videos_path = os.path.join(results_path, "Videos")

    os.mkdir(results_path)
    os.mkdir(a_path)
    os.mkdir(b_path)
    os.mkdir(videos_path)

    # Generating the output text files

    results_a_path  = os.path.join(a_path, "rewards.txt")
    results_b_path  = os.path.join(b_path, "rewards.txt")
    durations_path  = os.path.join(results_path, "durations.txt")
    outcomes_path   = os.path.join(results_path, "outcomes.txt")

    result_a    = open(results_a_path, "w+")
    result_b    = open(results_b_path, "w+")
    result_dur  = open(durations_path, "w+")
    result_wins = open(outcomes_path, "w+")

    # hyper parameters

    EPISODES = 100
    EPOCHS = 10
    save_image_epochs = [0, int((EPOCHS-1)/2), int(EPOCHS-1)]

    EPS_START = 0.9999
    EPS_DECAY = 0.999
    EPS_MIN = 0.1

    # environment

    env = Scenarios.ImageToPyTorch(Scenarios.Scenario182({}))
    env_2 = Scenarios.ImageToPyTorch(Scenarios.Scenario182NoRewards({}))

    env.game.set_max_fps(99999999)
    env.game.set_max_ups(99999999)

    env_2.game.set_max_fps(99999999)
    env_2.game.set_max_ups(99999999)

    # agent parameters

    state_size = env.observation_space.shape
    action_size = env.action_space.n

    # agents

    agent_a = Agents.DiegoConvAgent(state_size, action_size)
    agent_b = Agents.DiegoConvAgent(state_size, action_size)

    for epoch in range(EPOCHS):

        file_name = "epoch_" + str(epoch) + ".pt"

        print("Epoch: " + str(epoch))

        scores_a    = []
        scores_b    = []
        durations   = []
        results     = []

        duration = 0

        eps = EPS_START

        if (epoch == 2):
            env = env_2

        for episode in range(EPISODES):

            episode_start = datetime.now()

            fps = env.game.get_fps()
            ups = env.game.get_ups()
            print("Episode: %s, FPS: %s, UPS: %s, TIME: %s" % (episode, fps, ups, duration))

            terminal = False
            state = env.reset()

            score_a = 0
            score_b = 0

            while not terminal:

                # AI for player 1
                env.game.set_player(env.game.players[0])

                action = agent_a.get_action(state, eps)
                next_state, reward, terminal, info = env.step(action)
                agent_a.update(state, action, reward, next_state, terminal)

                score_a += reward

                # AI for player 1
                env.game.set_player(env.game.players[1])

                action = agent_b.get_action(state, eps)
                next_state, reward, terminal, info = env.step(action)
                agent_b.update(state, action, reward, next_state, terminal)

                score_b += reward

                # check if there is a winner
                if (env.game.is_terminal()):
                    if (env.game.players[0].is_defeated()):
                        results.append(0)
                    else:
                        results.append(1)

                eps = max(eps * EPS_DECAY, EPS_MIN)

                state = next_state

            scores_a.append(score_a)
            scores_b.append(score_b)

            durations.append(env.game.get_episode_duration())

            episode_end = datetime.now()

            duration = episode_end - episode_start

            durations.append(duration)

        # saving a copy of the neural network

        a_checkpoint = os.path.join(a_path, file_name)
        b_checkpoint = os.path.join(b_path, file_name)

        agent_a.save(a_checkpoint)
        agent_b.save(b_checkpoint)

        # saving results from previous epoch

        # saving agent a's rewards
        result_a.write("Epoch: " + str(epoch) + "\n")
        for score in scores_a:
            result_a.write(str(score) + ",")
        result_a.write("\n")
        result_a.flush()

        # saving agent b's rewards
        result_b.write("Epoch: " + str(epoch) + "\n")
        for score in scores_b:
            result_b.write(str(score) + ",")
        result_b.write("\n")
        result_b.flush()

        # saving duration of episodes
        result_dur.write("Epoch: " + str(epoch) + "\n")
        for duration in durations:
            result_dur.write(str(duration) + ",")
        result_dur.write("\n")
        result_dur.flush()

        # saving outcomes of episodes
        result_wins.write("Epoch: " + str(epoch) + "\n")
        for result in results:
            result_wins.write(str(result) + ",")
        result_wins.write("\n")
        result_wins.flush()

    result_a.close()
    result_b.close()
    print("Finished")
