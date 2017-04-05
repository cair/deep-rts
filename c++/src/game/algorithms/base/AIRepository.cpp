
#include "AIRepository.h"
#include "Algorithm.h"
#include <string>
#include <map>



void AIRepository::addAI(Algorithm *ai) {
	availableAI[ai->getName()] = ai;
}

Algorithm *AIRepository::getAI(std::string aiName) {
	Algorithm *ai = availableAI[aiName];
	assert(ai && "Invalid AI name. Doesnt exist! or DQN is NULLPTR");
	return ai;
}
