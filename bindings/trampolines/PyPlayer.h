//
// Created by Per-Arne on 23.02.2017.
//

#pragma once

#include "../../include/DeepRTS/Player.h"
using DeepRTS::Player;

using namespace DeepRTS;

class PyPlayer: public Player {
    using Player::Player;

    [[nodiscard]] Constants::PlayerState evaluatePlayerState() const override {
        PYBIND11_OVERRIDE(Constants::PlayerState, Player, evaluatePlayerState , );
    }

    [[nodiscard]] double getScore() const override {
        PYBIND11_OVERRIDE(int, Player, getScore, );
    }




};
