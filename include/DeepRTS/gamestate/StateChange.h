//
// Created by per on 23.03.18.
//

#ifndef DEEPRTS_STATECHANGE_H
#define DEEPRTS_STATECHANGE_H
namespace DeepRTS {

    class Game;

    class StateChange {
    public:
        virtual void apply(Game *game) {

        }

        virtual void apply_reverse(Game *game) {

        }
    };
}
#endif //DEEPRTS_STATECHANGE_H
