//
// Created by Per-Arne on 04.03.2017.
//

#ifndef WARC2SIM_MCTS_H
#define WARC2SIM_MCTS_H

#include "../base/Algorithm.h"
#include "MCTSNode.h"
#include <random>

class Game;
class MCTS: public Algorithm  {
public:
    MCTS(Player &player);
    virtual void update();
    virtual void terminal();
    virtual std::shared_ptr<BaseAction> findBestAction(Unit & unit);
    virtual void doAction(std::shared_ptr<BaseAction> action);
    virtual void defineActionSpace();
	std::shared_ptr<BaseAction> findBestAction();


    int timeBudget;
    int depthBudget;
	double epsilon;
	double epsilonModifier;
	double epsilonDecent;
	int playerID;
    MCTSNode root;

    void calculate(MCTSNode root);

    Game *game;
	std::mt19937 random_engine;
};


#endif //WARC2SIM_MCTS_H
