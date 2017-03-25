//
// Created by Per-Arne on 01.03.2017.
//

#include "Algorithm.h"
#include "../../player/Player.h"

Constants::Action Algorithm::getAction(int actionID)
{
	return static_cast<Constants::Action>(actionID);
}

void Algorithm::train()
{
	// Nothing to train
}

bool Algorithm::terminal()
{
	bool isTerminal = player->checkDefeat();
	return isTerminal;
}

void Algorithm::reset()
{
	// Nothing to do
}

void Algorithm::setPlayer(Player *player_) {
	player = player_;
}

void Algorithm::setPlayerName(std::string name){
    player->setName(name);
}

void Algorithm::setName(std::string name)
{
	name_ = name;
}

std::string Algorithm::getName()
{
	return name_;
}

std::shared_ptr<BaseAction> Algorithm::getAction(int actionID, Unit* unitptr) {
	if(!unitptr && !unitptr->tile) {
		std::cout << "Avoided crash ( Yes we have bugs, concurrency is gay..." << std::endl;
		return std::shared_ptr<BaseAction>();
	}
    return std::shared_ptr<BaseAction>();
}
