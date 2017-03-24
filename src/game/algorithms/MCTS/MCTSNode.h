//
// Created by Per-Arne on 04.03.2017.
//

#ifndef WARC2SIM_MCTSNODE_H
#define WARC2SIM_MCTSNODE_H


#include <memory>
#include <vector>

class MCTSNode;
class MCTSNode {
public:
    MCTSNode(MCTSNode *parent, int depth, int score);

    MCTSNode();
    MCTSNode *parent;
    std::vector<MCTSNode *> children;
    int score;
    int depth;

    int action = 0;

};


#endif //WARC2SIM_MCTSNODE_H
