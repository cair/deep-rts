//
// Created by Per-Arne on 04.03.2017.
//

#ifndef WARC2SIM_MCTSNODE_H
#define WARC2SIM_MCTSNODE_H


#include <memory>
#include <vector>
#include "../../proto/GameMessage.pb.h"

class MCTSNode;
class MCTSNode {
public:
    MCTSNode(std::shared_ptr<MCTSNode> parent, int depth, int score);

    MCTSNode();
	GameMessage gameState;
    std::shared_ptr<MCTSNode> parent;
    std::vector<std::shared_ptr<MCTSNode>> children;
    int score;
    int depth;

    int action = 0;

};


#endif //WARC2SIM_MCTSNODE_H
