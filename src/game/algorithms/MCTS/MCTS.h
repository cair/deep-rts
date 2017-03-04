//
// Created by Per-Arne on 04.03.2017.
//

#ifndef WARC2SIM_MCTS_H
#define WARC2SIM_MCTS_H

#include "../base/Algorithm.h"
#include "MCTSNode.h"
class Game;
class MCTS: public Algorithm  {
public:
    MCTS(Player &player);
    virtual void update();
    virtual void terminal();
    virtual std::shared_ptr<BaseAction> findBestAction(std::shared_ptr<Unit> unit);
    virtual void doAction(std::shared_ptr<BaseAction> action);
    virtual void defineActionSpace();

    int timeBudget;
    int depthBudget;
    MCTSNode root;

    void calculate(MCTSNode root);

    Game *game;
};


#endif //WARC2SIM_MCTS_H
