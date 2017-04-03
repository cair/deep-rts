//
// Created by Per-Arne on 04.03.2017.
//

#include "MCAS.h"
#include "../../player/Player.h"
#include "MCASNode.h"
#include "../../Game.h"
#include <SFML/System/Clock.hpp>
#include <algorithm>

MCAS::MCAS(Player *player) : Algorithm(player) {
    setName("MCAS");

    int _apm_interval = Config::getInstance().getAPM() * 60.0;

    nodes.reserve(100000);
    for(int i = 0; i < nodes.capacity(); i++) {
        nodes.push_back(MCASNode());
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

    actionSpaceStr = {
            "PrevUnit",
            "NextUnit",
            "MoveLeft",
            "MoveRight",
            "MoveUp",
            "MoveDown",
            "MoveUpLeft",
            "MoveUpRight",
            "MoveDownLeft",
            "MoveDownRight",
            "Attack",
            "Harvest",
            "Build0",
            "Build1",
            "Build2",
            "NoAction"
    };
    
    
    actionN = 0; // Set which action we are on (0 is first)
    // Prepopulate table
    actionTable.reserve(13000);
    for (int i = 0; i < 13000; i++){
        std::vector<double> data = std::vector<double>(actionSpace.size());
        std::fill(data.begin(), data.end(), 0);
        actionTable.push_back(data);
    }








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

MCASNode *MCAS::BestChildOfSumScore(MCASNode *node) {
    // Find best child based on the score sum

    MCASNode *best;
    double highest = 0;

    //std::cout << "---------\n-Summary\nNodes: " << node->children.size() <<  "\n---------" << std::endl;

    for(auto &ch : node->children) {
        MCASNode *n = &nodes[ch];
        //std::cout << n->sumScore << "("<< n->action <<")" << "("<< n->id <<")"" | ";
        if(n->sumScore > highest) {
            highest = n->sumScore;
            best = n;
        }

    }
    //std::cout << std::endl;

    return best;

}

MCASNode *MCAS::UCBSelection(MCASNode *node) {
    // UCB Formula:
    // vi + (C * sqrt(log(N) / ni));
    // vi - estimated value of the node (score)
    // C - Tunable parameter
    // N - Parent visits
    // ni - Number of visits


    MCASNode *best;
    double highest = -90000;

    for(auto &ch : node->children) {
        MCASNode *n = &nodes[ch];
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

void MCAS::calculate(){
    sf::Clock clock;
    sf::Time now = clock.getElapsedTime();
    auto nowMicroSec = now.asMicroseconds();
    auto timeout = nowMicroSec + timeBudget;




    std::uniform_real_distribution<> dist(0, 1);
    std::uniform_int_distribution<> actionDist(0, actionSpace.size() - 1);

    int currentDepth = 0;
    int iterator = 0;
    int nNodes = 0;


    MCASNode *currentNode = &nodes[nNodes++];
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
            MCASNode &leafNode = nodes[nNodes++];
            leafNode.children.clear(); // TODO slow?
            leafNode.childrenActions.clear();
            leafNode.action = randomAction;
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
                MCASNode *nextNode = &nodes.at(nodeIndex);

                currentNode = nextNode;

            } else {
                // have not sampled this action. EXPAND
                // Create new leaf node
                MCASNode &leafNode = nodes[nNodes++];
                leafNode.children.clear(); // TODO slow?
                leafNode.childrenActions.clear();
                leafNode.sumScore = 0;
                leafNode.action = randomAction;
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


            MCASNode *nextNode = UCBSelection(currentNode);
            assert(nextNode->id && "UCBSelection failed!");
            currentNode = nextNode;

            /*MCASNode *nextNode = BestChildOfSumScore(currentNode);
            currentNode = nextNode;*/

        }

        // Execute current node
        simPlayer->queueAction(static_cast<Constants::Action>(currentNode->action));


        // Simulate
        sim->loop();

        // Update counters
        currentDepth++;
        iterator++;

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


    auto bestNode = BestChildOfSumScore(&nodes[0]);
    Constants::Action bestAction;



    // ActionTable calculation
    auto max_value_it = std::max_element(actionTable[actionN].begin(), actionTable[actionN].end());
    double max_value = *max_value_it;


    if(max_value > bestNode->sumScore) {
        // Table has better action
        auto actionIndex = std::distance(std::begin(actionTable[actionN]), max_value_it);

        // TODO crashes?
        //std::cout << "Intervening: " << actionSpaceStr[bestNode->action] << " => " << actionSpaceStr[actionIndex] << " (" << actionIndex << ", " << actionN << ")"  << std::endl;
        bestAction = static_cast<Constants::Action>(actionIndex);
    }
    else {
        // MCTS found best action
        bestAction = static_cast<Constants::Action>(bestNode->action);
    }

    // Update table
    for(auto &childID : nodes[0].children) {
        MCASNode &childNode = nodes[childID];
        actionTable[actionN].at(childNode.action-1) = (actionTable[actionN].at(childNode.action-1) + childNode.sumScore) / 2;
    }



    actionN += 1;   // Update actionN counter
    player->queueAction(bestAction);

    // Tune epsilon
    epsilonModifier = (epsilonDecent + iterator) / 2;
    epsilonDecent =  epsilon / epsilonModifier;
    epsilon = startEpsilon;  // Reset epsilon;
    std::cout << "Iterations: " << std::to_string(iterator) << " | " << timeout / 1000 << "ms" <<std::endl;


}

int MCAS::findAction() {
    return 0;
}

void MCAS::train() {

}

void MCAS::reset() {
    actionN = 0;
}

void MCAS::update() {
    // Clear Node list
    //actionN = 0;
    sim->load(&player->game_);
    calculate();
}

bool MCAS::terminal() {
    return false;
}

