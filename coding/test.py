import Scenarios
import Agents

import torch
import imageio

import pygame

import os
from datetime import datetime

import numpy

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

	now = datetime.now()
	now_string = now.strftime("%d-%m-%Y %H-%M-%S")
	directory = "Videos " + now_string

	test_directory = "Tests"
	test_path = os.path.join(os.getcwd(), test_directory)

	files = ["NN_700"]

	results_path = os.path.join(os.getcwd(), "Results")
	recording_path = os.path.join(results_path, directory)
	log_path = os.path.join(recording_path, "log.txt")

	os.mkdir(recording_path)
	log = open(log_path, "w+")

	# environment

	env = Scenarios.ImageToPyTorch(Scenarios.Scenario182({}))

	env.game.set_max_fps(99999999)
	env.game.set_max_ups(99999999)

	TRIALS = 100

	for file in files:

		file_path = os.path.join(test_path, file + ".pt")

		results_directory = file + "-Random"
		results_path = os.path.join(test_path, results_directory)
		os.mkdir(results_path)

		outcomes_path = os.path.join(results_path, "outcomes.txt")
		durations_path = os.path.join(results_path, "durations.txt")

		outcomes_file = open(outcomes_path, "w+")
		durations_file = open(durations_path, "w+")

		# agents

		state_size = env.observation_space.shape
		action_size = env.action_space.n

		agent_a = Agents.SmallAgent(4410, action_size)
		agent_a.load(file_path)

		agent_b = Agents.RandomAgent()

		for trial in range(TRIALS):

			state = env.reset()
			flat_state = state.flatten()

			# video stuff

			filenames = []

			terminal = False
			changed = False
			count = 0

			# play game
			while not terminal:

				if trial == 0:
					if changed:
						# save the current window
						window = pygame.display.get_surface()

						image_name = "image_" + str(count) + ".jpeg"
						image_path = os.path.join(results_path, image_name)
						pygame.image.save(window, image_path)

						filenames.append(image_path)

						count += 1

				# AI for player 1
				env.game.set_player(env.game.players[0])

				action = agent_a.get_action(flat_state, 0)
				next_state, _, terminal, _ = env.step(action)
				flat_next_state = next_state.flatten()

				# AI for player 1
				env.game.set_player(env.game.players[1])

				action = agent_b.get_action(state, 0)
				next_state, _, terminal, _ = env.step(action)

				changed = not numpy.array_equal(state, next_state)

				state = next_state
				flat_state = flat_next_state

			if (env.game.players[0].is_defeated()):
				outcomes_file.write("0,")
				outcomes_file.flush()
			else:
				outcomes_file.write("1,")
				outcomes_file.flush()

			durations_file.write(str(env.game.get_episode_duration()) + ",")
			durations_file.flush()

			if trial == 0:
				images = []
				for filename in filenames:
					images.append(imageio.imread(filename))
				video_path = os.path.join(results_path, "video.gif")
				imageio.mimsave(video_path, images)