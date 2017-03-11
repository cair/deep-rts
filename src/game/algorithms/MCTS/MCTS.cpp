//
// Created by Per-Arne on 04.03.2017.
//

#include "MCTS.h"
#include "../../player/Player.h"
#include "MCTSNode.h"
#include "../../Game.h"

MCTS::MCTS(Player &player) : Algorithm(player) {

    timeBudget = 1000; // 1 second
    depthBudget = 10; // Depth budget (Tree depth)
	epsilon = 0.9;	// Start of epsilon
	epsilonModifier = 1000000; // Number of iterations to reach epsilon 0.1
	epsilonDecent = epsilon / epsilonModifier; // How much epsilon decrease with per iteration
	playerID = player.id_;  // Index of selected player


    game = new Game(4, false);
    game->deactivateGUI();
    game->start();

}

void MCTS::calculate(MCTSNode root){
	clock_t now = clock();
	clock_t timeout = now + timeBudget;

	MCTSNode &current = root;
	// Load up game state
	long counter = 0;

	while (now < timeout) {
		now = clock();

		

		/*if (current.children.size() == 0) {
			// Has no children, must select random action

		}
		else*/ if (std::uniform_real_distribution<>(0.0, 1.0)(random_engine) < epsilon) {
			// Explore (Random Action)
			auto action = findBestAction();

			std::cout << epsilon << std::endl;
			epsilon -= epsilonDecent;
		}
		else {
			// Exploit (Use domain experience)

		}



		counter++;
	}


	std::cout << "Iterations: " << std::to_string(counter) << std::endl;
	

}


void MCTS::update() {
    // Tick happended

    root = MCTSNode(NULL, 0, 0);

    calculate(root);




}

void MCTS::terminal() {

}

std::shared_ptr<BaseAction> MCTS::findBestAction() {
	///
	/// Find best move for current state
	///

	return std::shared_ptr<BaseAction>(NULL);
}

std::shared_ptr<BaseAction> MCTS::findBestAction(Unit & unit) {
	///
	/// Find best move for current state
	///

	return std::shared_ptr<BaseAction>(NULL);
}


void MCTS::doAction(std::shared_ptr<BaseAction> action) {

}

void MCTS::defineActionSpace() {

}
