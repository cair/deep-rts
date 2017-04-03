//
// Created by per-arne on 30.03.17.
//

#include "MCTSDirect.h"
//
// Created by Per-Arne on 04.03.2017.
//
#include "../../player/Player.h"
#include "../../Game.h"
#include "../../util/Pathfinder.h"
#include <algorithm>
MCTSDirect::MCTSDirect(Player *player) : Algorithm(player) {
    setName("MCTSDirect");


    int _apm_interval = Config::getInstance().getAPM() * 60.0;

    nodes.reserve(100000);
    for(int i = 0; i < nodes.capacity(); i++) {
        nodes.push_back(MCTSNode());
        nodes.back().id = i;
    }

    startEpsilon = 0.9;

    timeBudget = _apm_interval; // 1 second
    depthBudget = 20; // Depth budget (Tree depth)
    epsilon = startEpsilon;	// Start of epsilon
    epsilonModifier = 1000000; // Number of iterations to reach epsilon 0.1
    epsilonDecent = epsilon / epsilonModifier; // How much epsilon decrease with per iteration
    playerID = player->id_;  // Index of selected player


    actionSpace = {
            Constants::Action::PreviousUnit,
            Constants::Action::NextUnit,

            Constants::Action::MoveLeft,
            Constants::Action::MoveRight,
            Constants::Action::MoveUp,
            Constants::Action::MoveDown,
            Constants::Action::MoveUpLeft,
            Constants::Action::MoveUpRight,
            Constants::Action::MoveDownLeft,
            Constants::Action::MoveDownRight,

            Constants::Action::Attack,
            Constants::Action::Harvest,

            Constants::Action::Build0,
            Constants::Action::Build1,
            Constants::Action::Build2,
            Constants::Action::NoAction
    };

    sim = new Game(4, false);
    sim->addPlayer();
    sim->addPlayer();
    sim->addPlayer();
    sim->addPlayer();

    sim->deactivateGUI();
    //sim->doDisplay = true;
    sim->initGUI();
    sim->stop(); // Set Running to false. This allows loop() to only run once (Iterate)
    sim->setFPS(1);
    sim->setUPS(INT32_MAX);

    // Set simulation player
    simPlayer = &sim->players[playerID];

}

MCTSNode *MCTSDirect::BestChildOfSumScore(MCTSNode *node) {
    // Find best child based on the score sum

    MCTSNode *best;
    double highest = 0;

    //std::cout << "---------\n-Summary\nNodes: " << node->children.size() <<  "\n---------" << std::endl;

    for(auto &ch : node->children) {
        MCTSNode *n = &nodes[ch];
        //std::cout << n->sumScore << "("<< n->action <<")" << "("<< n->id <<")"" | ";
        if(n->sumScore > highest) {
            highest = n->sumScore;
            best = n;
        }

    }
    //std::cout << std::endl;

    return best;

}

MCTSNode *MCTSDirect::UCBSelection(MCTSNode *node) {
    // UCB Formula:
    // vi + (C * sqrt(log(N) / ni));
    // vi - estimated value of the node (score)
    // C - Tunable parameter
    // N - Parent visits
    // ni - Number of visits


    MCTSNode *best;
    double highest = -90000;

    for(auto &ch : node->children) {
        MCTSNode *n = &nodes[ch];
        double ucb = n->score + (1 * sqrt(log(node->visited) / n->visited));

        //std::cout << ucb << std::endl;
        if(ucb > highest) {
            highest = ucb;
            best = n;
        }

    }

    //std::cout << "-----" << std::endl;
    return best;
}

void MCTSDirect::calculate(){
    sf::Clock clock;
    sf::Time now = clock.getElapsedTime();
    auto nowMicroSec = now.asMicroseconds();
    auto timeout = nowMicroSec + timeBudget;




    std::uniform_real_distribution<> dist(0, 1);
    std::uniform_int_distribution<> actionDist(0, actionSpace.size() - 1);

    int currentDepth = 0;
    int iterator = 0;
    int nNodes = 0;


    MCTSNode *currentNode = &nodes[nNodes++];
    currentNode->children.clear();
    currentNode->childrenActions.clear();
    currentNode->score = 1; // simPlayer->getScore();
    currentNode->action = -1;
    currentNode->parent = NULL;
    currentNode->depth = currentDepth;



    while (nowMicroSec < timeout) {
        now = clock.getElapsedTime();		// Update clock
        nowMicroSec = now.asMicroseconds();

        /////////////////////////////////////////////////////////
        ///
        /// Exploration via random sampling (Expansion)
        /// - This happens when currentNode is a leaf node
        /////////////////////////////////////////////////////////
        if (currentNode->children.empty()) {

            // Generate random action
            int randomAction = actionSpace[actionDist(random_engine)];

            // Create new leaf node
            MCTSNode &leafNode = nodes[nNodes++];
            leafNode.children.clear(); // TODO slow?
            leafNode.childrenActions.clear();
            leafNode.action = randomAction;
            leafNode.actionTypeDEBUG = (Constants::Action) randomAction;
            leafNode.depth = currentDepth;
            leafNode.sumScore = 0;
            leafNode.score = simPlayer->getScore() - currentNode->score; // TODO this score is before action is performed Must have temporal stuff between this and action


            // Update parent and children
            leafNode.parent = currentNode->id;
            currentNode->children.push_back(leafNode.id);
            currentNode->childrenActions.push_back(leafNode.action);

            // Set currentNode
            currentNode = &leafNode;
        }

            /////////////////////////////////////////////////////////
            ///
            /// Exploration via random sampling (Existing child)
            /// - The point of this is to explore a already known state
            /////////////////////////////////////////////////////////
        else if (dist(random_engine) < epsilon) {

            // Generate random action
            int randomAction = actionSpace[actionDist(random_engine)];

            // If randomAction is in children, use that, else create new leafNode
            //auto it = currentNode->childrenActions.find(randomAction);
            auto it = std::find(currentNode->childrenActions.begin(), currentNode->childrenActions.end(), randomAction);
            const bool is_in = it != currentNode->childrenActions.end();
            if(is_in) {
                // have already sampled this action
                auto index = std::distance(currentNode->childrenActions.begin(), it);
                auto nodeIndex = currentNode->children[index];
                MCTSNode *nextNode = &nodes.at(nodeIndex);

                currentNode = nextNode;

            } else {
                // have not sampled this action. EXPAND
                // Create new leaf node
                MCTSNode &leafNode = nodes[nNodes++];
                leafNode.children.clear(); // TODO slow?
                leafNode.childrenActions.clear();
                leafNode.sumScore = 0;
                leafNode.action = randomAction;
                leafNode.actionTypeDEBUG = (Constants::Action) randomAction;
                leafNode.depth = currentDepth;
                leafNode.score = simPlayer->getScore() - currentNode->score; // TODO this score is before action is performed Must have temporal stuff between this and action
                // Update parent and children
                leafNode.parent = currentNode->id;
                currentNode->children.push_back(leafNode.id);
                currentNode->childrenActions.push_back(leafNode.action);

                // Set currentNode
                currentNode = &leafNode;
            }




            epsilon -= epsilonDecent;
        }

            /////////////////////////////////////////////////////////
            ///
            /// Exploitation of knowledge (Exploit)
            ///
            /////////////////////////////////////////////////////////
        else {


            MCTSNode *nextNode = UCBSelection(currentNode);
            assert(nextNode->id && "UCBSelection failed!");
            currentNode = nextNode;

            /*MCTSNode *nextNode = BestChildOfSumScore(currentNode);
            currentNode = nextNode;*/

        }

        // Execute current node
        simPlayer->queueAction(static_cast<Constants::Action>(currentNode->action));


        // Simulate
        sim->loop();

        // Update counters
        currentDepth++;
        iterator++;
        bestActionQueue.put(currentNode, currentNode->score);

        if(currentDepth >= depthBudget) {
            int sum = 0;
            // Propagate back up the tree
            while(currentNode->id != 0) {

                // Increment visitor count
                currentNode->visited += 1;

                // Increment sum score for parent
                auto &parent = nodes[currentNode->parent];
                sum += currentNode->score;
                parent.sumScore = sum;


                // Set currentNode to parent
                currentNode = &parent;
            }
            currentNode->visited ++;
            currentDepth = 0;
            sim->load(&player->game_);



        }


    }


    auto bestNode = bestActionQueue.get();
    //player->queueAction(static_cast<Constants::Action >(bestNode->action));
    translateAction(bestNode);
    bestActionQueue.clear();
    std::cout << "Direct Iterations: " << std::to_string(iterator) << " | " << timeout / 1000 << "ms" <<std::endl;

    // Tune epsilon
    epsilonModifier = (epsilonDecent + iterator) / 2;
    epsilonDecent =  epsilon / epsilonModifier;
    epsilon = startEpsilon;  // Reset epsilon;



}

void MCTSDirect::translateAction(MCTSNode *pNode) {
    ///
    /// We translate the action by doing the following:
    /// Identify selectedUnit and its position
    /// Convert action to a rightClick based on which type of action it is
    /// The return is an rightClickAction

    Unit *selectedUnit = simPlayer->getTargetedUnit();
    if(!selectedUnit) {
        player->queueAction(Constants::Action::NextUnit);
        return;
    }
    Tile* unitTile = selectedUnit->tile;
    if(!unitTile) {
        player->queueAction(Constants::Action::NoAction);
        return;
    }
    Position rightClickAction;
    rightClickAction = unitTile->getPosition();

    if(Constants::Action::Attack == pNode->action) {
        // Find an attackable unit nearby (neighbour) and set rightclick as this position
        Tile* target = Pathfinder::find_first_attackable_tile(unitTile);
        if(target) {
            // Attack this target
            player->rightClick(target->getPosition());
        } else {
            // There was no target to find, anyhow, move unit to simulators position
            player->rightClick(unitTile->getPosition());
            player->queueAction(Constants::Action::Attack);
        }
    }
    else if(Constants::Action::Build0 == pNode->action) {
        // Enqueue move to this position
        player->rightClick(unitTile->getPosition());
        player->queueAction(Constants::Action::Build0);
    }
    else if(Constants::Action::Build1 == pNode->action) {
        // Enqueue move to this position
        player->rightClick(unitTile->getPosition());
        player->queueAction(Constants::Action::Build1);
    }
    else if(Constants::Action::Build2 == pNode->action) {
        // Enqueue move to this position
        player->rightClick(unitTile->getPosition());
        player->queueAction(Constants::Action::Build2);
    }
    else if(Constants::Action::Harvest == pNode->action) {
        // Find an harvestable tile nearby (neighbour) and set rightclick as this position
        Tile* targetTile = Pathfinder::find_first_harvestable_tile(unitTile);
        if(targetTile) {
            player->rightClick(targetTile->getPosition());
        } else {
            player->rightClick(unitTile->getPosition());
            player->queueAction(Constants::Action::Harvest);
        }

    }
    else if(Constants::Action::MoveDown == pNode->action) {
        // Move to this position + down
        rightClickAction.y += 1;
        player->rightClick(rightClickAction);
    }
    else if(Constants::Action::MoveDownLeft == pNode->action) {
        rightClickAction.x -= 1;
        rightClickAction.y += 1;
        player->rightClick(rightClickAction);
    }
    else if(Constants::Action::MoveDownRight == pNode->action) {
        rightClickAction.x += 1;
        rightClickAction.y += 1;
        player->rightClick(rightClickAction);
    }
    else if(Constants::Action::MoveLeft == pNode->action) {
        rightClickAction.x -= 1;
        player->rightClick(rightClickAction);
    }
    else if(Constants::Action::MoveRight == pNode->action) {
        rightClickAction.x += 1;
        player->rightClick(rightClickAction);
    }
    else if(Constants::Action::MoveUp == pNode->action) {
        rightClickAction.y -= 1;
        player->rightClick(rightClickAction);
    }
    else if(Constants::Action::MoveUpLeft == pNode->action) {
        rightClickAction.x -= 1;
        rightClickAction.y -= 1;
        player->rightClick(rightClickAction);
    }
    else if(Constants::Action::MoveUpRight == pNode->action) {
        rightClickAction.x += 1;
        rightClickAction.y -= 1;
        player->rightClick(rightClickAction);
    }
    else if(Constants::Action::NextUnit == pNode->action) {
        // Move to this unit and do next unit
        player->rightClick(rightClickAction);
        player->queueAction(Constants::Action::NextUnit);
    }
    else if(Constants::Action::PreviousUnit == pNode->action) {
        // Move to this unit and do prev unit
        player->rightClick(rightClickAction);
        player->queueAction(Constants::Action::PreviousUnit);
    }
    else if(Constants::Action::NoAction == pNode->action) {
        // Move to this unit position
        player->rightClick(rightClickAction);
    }


    //pNode->action




}



int MCTSDirect::findAction() {
    return 0;
}

void MCTSDirect::train() {

}

void MCTSDirect::reset() {

}

void MCTSDirect::update() {
    // Clear Node list
    sim->load(&player->game_);
    calculate();
}

bool MCTSDirect::terminal() {
    return false;
}


