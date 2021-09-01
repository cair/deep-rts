//
// Created by Per-Arne on 23.02.2017.
//

#pragma once

#include "../../include/DeepRTS/Player.h"
class PyPlayer: public Player {
    using Player::Player;

    Constants::PlayerState playerState() override {
        PYBIND11_OVERLOAD(Constants::PlayerState, Player, playerState, );
    }

    [[nodiscard]] int getScore() const override {
        PYBIND11_OVERLOAD(int, Player, getScore, );
    }




};
