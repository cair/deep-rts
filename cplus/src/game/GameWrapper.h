//
// Created by per on 27.08.17.
//

#ifndef DEEPRTS_PYTHONGAME_H
#define DEEPRTS_PYTHONGAME_H


class Game;
class GameWrapper {


    Game *game;

public:
    GameWrapper();

    void loop();
    void start();
    void initGUI();

};


#endif //DEEPRTS_PYTHONGAME_H
