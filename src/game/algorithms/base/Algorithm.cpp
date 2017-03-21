//
// Created by Per-Arne on 01.03.2017.
//

#include "Algorithm.h"
#include "../../player/Player.h"

void Algorithm::setPlayerName(std::string name){
    player->setName(name);
}

std::shared_ptr<BaseAction> Algorithm::getAction(int actionID, Unit* unitptr) {
	if(!unitptr && !unitptr->tile) {
		std::cout << "Avoided crash ( Yes we have bugs, concurrency is gay..." << std::endl;
		return std::shared_ptr<BaseAction>();
	}
    return std::shared_ptr<BaseAction>();
}
