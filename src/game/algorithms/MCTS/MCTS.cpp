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

    game = new Game(4, false);
    game->deactivateGUI();
    game->start();

}

void MCTS::calculate(MCTSNode root){



}


void MCTS::update() {
    // Tick happended

    root = MCTSNode(NULL, 0, 0);

    calculate(root);




}

void MCTS::terminal() {

}

std::shared_ptr<BaseAction> MCTS::findBestAction(std::shared_ptr<Unit> unit) {

}

void MCTS::doAction(std::shared_ptr<BaseAction> action) {

}

void MCTS::defineActionSpace() {

}
