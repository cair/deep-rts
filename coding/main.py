
# Run setup.py install in sources/python to build source files.

from DeepRTS.python import scenario
from DeepRTS.Engine import Random

import scenario182
import dqn_agent
import random_agent

if __name__ == "__main__":
    result = open("result.txt", "w")
    episodes = 100
    epochs = 10
    random_play = True

    env = scenario182.Scenario182({})

    env.game.set_max_fps(99999999)
    env.game.set_max_ups(99999999)

    state_size = env.observation_space.shape[0]
    action_size = env.action_space.n

    # agent_a = random_agent.RandomAgent()
    agent_a = dqn_agent.DQNAgent(state_size, action_size, seed=0)
    # agent_b = random_agent.RandomAgent()
    agent_b = dqn_agent.DQNAgent(state_size, action_size, seed=0)

    result_a = open("/Users/diegogutierrez/Documents/college/semester_3/COMPSCI_182/final_project/myfork/results/agent_a/result.txt", "w+")
    result_b = open("/Users/diegogutierrez/Documents/college/semester_3/COMPSCI_182/final_project/myfork/results/agent_b/result.txt", "w+")
    result_dur = open("/Users/diegogutierrez/Documents/college/semester_3/COMPSCI_182/final_project/myfork/results/durations.txt", "w+")
    result_wins = open("/Users/diegogutierrez/Documents/college/semester_3/COMPSCI_182/final_project/myfork/results/results.txt", "w+")

    for epoch in range(epochs):

        file_name = "epoch_" + str(epoch) + ".pt"

        print("Epoch: " + str(epoch))

        scores_a = []
        scores_b = []
        durations = []
        results = []

        for episode in range(episodes):
            print("Episode: %s, FPS: %s, UPS: %s" % (episode, env.game.get_fps(), env.game.get_ups()))
            terminal = False
            state = env.reset()
            score_a = 0
            score_b = 0

            count = 0
            prev_damage = 0
            prev_units = 0

            while not terminal:

                # AI for player 1
                env.game.set_player(env.game.players[0])

                action = agent_a.get_action(state)
                next_state, reward, terminal, info = env.step(action)
                agent_a.update(state, action, reward, next_state, terminal)

                score_a += reward


                # AI for player 1
                env.game.set_player(env.game.players[1])

                action = agent_b.get_action(state)
                next_state, reward, terminal, info = env.step(action)
                agent_b.update(state, action, reward, next_state, terminal)

                score_b += reward

                if (env.game.is_terminal()):
                    if (env.game.players[0].is_defeated()):
                        results.append(0)
                    else:
                        results.append(1)

                state = next_state

            scores_a.append(score_a)
            scores_b.append(score_b)

            durations.append(env.game.get_episode_duration())


        agent_a.save("/Users/diegogutierrez/Documents/college/semester_3/COMPSCI_182/final_project/myfork/results/agent_a/" + file_name)
        agent_b.save("/Users/diegogutierrez/Documents/college/semester_3/COMPSCI_182/final_project/myfork/results/agent_b/" + file_name)

        result_a.write("Epoch: " + str(epoch) + "\n")
        for score in scores_a:
            result_a.write(str(score) + ",")
        result_a.write("\n")

        result_b.write("Epoch: " + str(epoch) + "\n")
        for score in scores_b:
            result_b.write(str(score) + ",")
        result_b.write("\n")

        result_dur.write("Epoch: " + str(epoch) + "\n")
        for duration in durations:
            result_dur.write(str(duration) + ",")
        result_dur.write("\n")

        result_wins.write("Epoch: " + str(epoch) + "\n")
        for result in results:
            result_wins.write(str(result) + ",")
        result_wins.write("\n")

    result_a.close()
    result_b.close()
    print("Finished")