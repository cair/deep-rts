//
// Created by Per-Arne on 01.03.2017.
//

#ifndef WARC2SIM_ALGORITHM_H
#define WARC2SIM_ALGORITHM_H

#include <string>
#include "../../unit/Unit.h"
#include "../../action/BaseAction.h"

class Player;
class Action;
class Algorithm {
private:
	std::string name_;

protected:
    Player *player = NULL;
	
    std::vector<Constants::Action> actionSpace;

public:
    Algorithm(Player *player): player(player){
        defineActionSpace();
    };



    virtual void update() = 0;	// Executes a iteration
	virtual int findAction() = 0; // Determine which action
	Constants::Action getAction(int actionID);

	virtual void train();
	virtual bool terminal();
    virtual void defineActionSpace(){};
	
	virtual void reset();

	void setPlayer(Player *player_);
    void setPlayerName(std::string name);
	void setName(std::string name);
	std::string getName();


    std::shared_ptr<BaseAction> getAction(int actionID, Unit *unitptr);
};


#endif //WARC2SIM_ALGORITHM_H
