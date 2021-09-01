//
// Created by per on 3/16/17.
//

#ifndef DEEPRTS_POSITION_H
#define DEEPRTS_POSITION_H

struct Position {
    Position(){};
    Position(int x, int y): x(x), y(y){};

    int x;
    int y;
};

#endif //DEEPRTS_POSITION_H
