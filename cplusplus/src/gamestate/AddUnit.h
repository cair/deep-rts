//
// Created by per on 23.03.18.
//

#ifndef DEEPRTS_ADDUNIT_H
#define DEEPRTS_ADDUNIT_H


#include "StateChange.h"

class Game;
class AddUnit: StateChange{
    int id;
    int type;
    int player_id;

    AddUnit(int id, int type, int player_id): id(id), type(type), player_id(player_id){}


    void apply(Game *game);

    void apply_reverse(Game *game);
};


#endif //DEEPRTS_ADDUNIT_H
