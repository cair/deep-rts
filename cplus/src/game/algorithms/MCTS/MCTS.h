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
    MCTS(Player *player);

	Player *simPlayer;

	virtual void update();	// Executes a iteration
	virtual int findAction(); // Determine which action
	virtual void train();
	virtual bool terminal();
	virtual void defineActionSpace(){};
	virtual void reset();


    int timeBudget;
    int depthBudget;
	double startEpsilon;
	double epsilon;
	double epsilonModifier;
	double epsilonDecent;
	int playerID;
    MCTSNode root;

    void calculate();
	int randomAction();
	MCTSNode *UCBSelection(MCTSNode *node);
	MCTSNode *BestChildOfSumScore(MCTSNode *node);

	std::vector<MCTSNode> nodes;

    Game *sim;
	std::mt19937 random_engine;
};


#endif //WARC2SIM_MCTS_H
