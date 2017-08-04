//
// Created by Per-Arne on 04.03.2017.
//

#ifndef WARC2SIM_MCAS_H
#define WARC2SIM_MCAS_H

#include "../base/Algorithm.h"
#include "MCASNode.h"
#include <random>

class Game;
class MCAS: public Algorithm  {
public:
    MCAS(Player *player);

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
    MCASNode root;
	std::vector<std::vector<double>> actionTable;
	std::vector<std::string> actionSpaceStr;
	int actionN;

    void calculate();
	int randomAction();
	MCASNode *UCBSelection(MCASNode *node);
	MCASNode *BestChildOfSumScore(MCASNode *node);

	std::vector<MCASNode> nodes;

    Game *sim;
	std::mt19937 random_engine;
};


#endif //WARC2SIM_MCAS_H
