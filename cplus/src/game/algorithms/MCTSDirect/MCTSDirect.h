//
// Created by Per-Arne on 04.03.2017.
//

#pragma once

#include "../base/Algorithm.h"
#include "../MCTS/MCTSNode.h"
#include <random>
#include "../../util/PriorityQueue.hpp"

class Game;
class MCTSDirect: public Algorithm  {
public:
    MCTSDirect(Player *player);

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
    PriorityQueue<MCTSNode*, int > bestActionQueue;

    Game *sim;
    std::mt19937 random_engine;

    void translateAction(MCTSNode *pNode);
};
