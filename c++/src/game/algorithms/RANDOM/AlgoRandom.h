//
// Created by Per-Arne on 01.03.2017.
//

#ifndef WARC2SIM_ALGORANDOM_H
#define WARC2SIM_ALGORANDOM_H


#include "../base/Algorithm.h"
#include <random>

class AlgoRandom: public Algorithm{
public:
    AlgoRandom(Player *player);

	virtual void update();
	virtual int findAction();
    virtual void defineActionSpace();


	std::random_device rd;
	std::mt19937 rgen;

};


#endif //WARC2SIM_ALGORANDOM_H
