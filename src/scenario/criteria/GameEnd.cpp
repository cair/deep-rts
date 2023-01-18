//
// Created by per on 9/9/21.
//

#include "scenario/criterias/GameEnd.h"
#include "Player.h"
#include "Game.h"
bool DeepRTS::Criteria::GameEnd::evaluate(const Player &player) {
    rewardable = player.getState() == Constants::PlayerState::Playing;

    return std::all_of(player.getGame().players.begin(), player.getGame().players.end(), [](auto&p){
        return p.getState() == Constants::PlayerState::Defeat;
    });

}

int DeepRTS::Criteria::GameEnd::reward() const {
    return (rewardable) ? rewardSuccess : rewardFailure;
}

DeepRTS::Criteria::GameEnd::GameEnd(int rewardSuccess, int rewardFailure)
: ScenarioCriteria(rewardSuccess, rewardFailure)
{

}
