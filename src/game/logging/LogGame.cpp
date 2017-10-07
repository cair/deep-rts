//
// Created by per-arne on 03.04.17.
//

#include <chrono>
#include "../Game.h"
#include "LogGame.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>


LogGame::LogGame(Game &game): game(game) {
    for(auto i = 0; i < 16; i++){
        players.push_back(LogPlayer());
    }

//gameNum, "games/deeprts_game_" + std::to_string(gameNum) + "_" + std::to_string(ms.count())
}

void LogGame::serialize() {


    // Record game names
    for (int i = 0; i < game.players.size(); i++){
        auto &logP = players.at(i);
        auto &p = game.players[i];
        logP.name = p.name_;
    }

    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.SetObject();
    rapidjson::Value playerArr(rapidjson::kArrayType);

    for(auto &p : players) {
        if(p.ticks.size() == 0)
            break;

        rapidjson::Value pObj(rapidjson::kObjectType);

        rapidjson::Value ticks(rapidjson::kArrayType);
        rapidjson::Value name;
        name.SetString(rapidjson::StringRef(p.name.c_str(), p.name.length()));

        for(auto &i: p.ticks){
            rapidjson::Value tick(rapidjson::kObjectType);


            rapidjson::Value apm(i.apm);
            rapidjson::Value score(i.score);
            rapidjson::Value aDist(rapidjson::kArrayType);
            for(auto d : i.actionDist) {
                aDist.PushBack(d, allocator);
            }

            tick.AddMember("apm", apm, allocator);
            tick.AddMember("score", score, allocator);
            tick.AddMember("dist", aDist, allocator);

            ticks.PushBack(tick, allocator);
        }

        pObj.AddMember("name", name, allocator);
        pObj.AddMember("ticks", ticks, allocator);

        playerArr.PushBack(pObj, allocator);
    }

    doc.AddMember("players", playerArr, allocator);
    doc.AddMember("game_num", game.getGameCount(), allocator);



    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    std::ofstream ofs("games//game_" + std::to_string(ms.count()) + ".json");
    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    doc.Accept(writer);
}

void LogGame::reset() {
    doc.RemoveMember("players");
    doc.RemoveMember("game_num");
    for(auto &p : players) {
        p.ticks.clear();
    }

}

void LogGame::record() {
    for (int i = 0; i < game.players.size(); i++){
        auto &logP = players.at(i);
        auto &p = game.players[i];

        auto tickItem = LogTick();
        tickItem.apm = p.apm;
        std::copy(std::begin(p.actionStatistics), std::end(p.actionStatistics), std::begin(tickItem.actionDist));
        tickItem.score = p.getScore();
        logP.ticks.push_back(tickItem);
    }
}

