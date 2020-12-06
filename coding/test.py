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

	results_path = os.path.join(os.getcwd(), "Results")
	recording_path = os.path.join(results_path, "Video")
	log_path = os.path.join(recording_path, "log.txt")

	os.mkdir(recording_path)
	log = open(log_path, "w+")

	# environment

	env = Scenarios.Scenario182({})

	env.game.set_max_fps(99999999)
	env.game.set_max_ups(99999999)

	state = env.reset()

	print(type(state))

	# agents

	agent_a = Agents.RandomAgent()
	agent_b = Agents.RandomAgent()

	# video stuff

	filenames = []

	terminal = False
	changed = False
	count = 0

	# play game
	while not terminal:

		if changed:
			# save the current window
			window = pygame.display.get_surface()

			image_name = "image_" + str(count) + ".jpeg"
			image_path = os.path.join(recording_path, image_name)
			pygame.image.save(window, image_path)

			filenames.append(image_path)

			count += 1

		# AI for player 1
		env.game.set_player(env.game.players[0])

		action = agent_a.get_action(state, 0)
		next_state, _, terminal, _ = env.step(action)

		log.write(action_names.get(action + 1) + ",")

		# AI for player 1
		env.game.set_player(env.game.players[1])

		action = agent_b.get_action(state, 0)
		next_state, _, terminal, _ = env.step(action)

		log.write(action_names.get(action + 1) + "\n")

		changed = not numpy.array_equal(state, next_state)

		state = next_state

	images = []
	for filename in filenames:
		images.append(imageio.imread(filename))
	video_path = os.path.join(recording_path, "video.gif")
	imageio.mimsave(video_path, images)