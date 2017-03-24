//
// Created by Per-Arne on 04.03.2017.
//

#include "MCTSNode.h"

MCTSNode::MCTSNode(MCTSNode *parent, int depth, int score): parent(parent), depth(depth), score(score) {

}

MCTSNode::MCTSNode() {}
