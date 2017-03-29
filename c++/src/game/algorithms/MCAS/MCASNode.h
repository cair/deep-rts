//
// Created by Per-Arne on 04.03.2017.
//

#ifndef WARC2SIM_MCASNODE_H
#define WARC2SIM_MCASNODE_H


#include <memory>
#include <set>
#include <vector>

class MCASNode;
class MCASNode {
public:

    MCASNode();
    int id;
    int parent;
    std::vector<int> children;
    std::vector<int> childrenActions;
    int score;
    int depth;
    int action = 0;
    int visited = 0;
    int sumScore = 0;

};


#endif //WARC2SIM_MCASNODE_H
