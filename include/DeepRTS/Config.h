//
// Created by per on 12.02.18.
//

#pragma once
#include <spdlog/spdlog.h>

class Config {
private:

    float _boundLimits(float v, float min, float max){
        if(v > max){
            v = max;
            SPDLOG_WARN("Yield modifier can not be above 10.0");
        }else if(v < min){
            v = min;
            SPDLOG_WARN("Yield modifier can not be below 10.0");
        }
        return v;
    }

public:


    int tickModifier = 10;
    bool instantTownHall = true;
    bool instantBuilding = true;
    bool harvestForever = false;
    bool autoAttack = true;
    int foodLimit = 100;
    bool farmEnabled = true;
    bool barracksEnabled = true;
    bool footmanEnabled = true;
    bool archerEnabled = false;
    bool pompd = false;
    std::string gui = "Blend2DGui";  // No need to export because it is used to initiate gui on C++ side only
    int startGold = 0;
    int startStone = 0;
    int startLumber = 0;
    int startFood = 1;
    int terminalSignal = true;
    float yieldModifierGold = 1.0;
    float yieldModifierStone = 1.0;
    float yieldModifierLumber = 1.0;
    bool animationEnabled = true; // ~10% perf decay.

    /// Bool that determine print of FPS and UPS in console
    bool consoleCaptionEnabled = true;

    void setAnimationEnabled(bool v){
        animationEnabled = v;
    }

    void setGUI(const std::string& b){
        gui = b;
    }

    void setYieldModifierGold(float mod){
        yieldModifierGold = _boundLimits(mod, 0.0, 10.0);
    }

    void setYieldModifierStone(float mod){
        yieldModifierStone = _boundLimits(mod, 0.0, 10.0);
    }

    void setYieldModifierLumber(float mod){
        yieldModifierLumber = _boundLimits(mod, 0.0, 10.0);
    }

    void setConsoleCaptionEnabled(bool b) {
        consoleCaptionEnabled = b;
    }

    void setPOMDP(bool b){
        pompd = b;
    }

    void setTickModifier(int tickmodifier){
        tickModifier = tickmodifier;
    }

    void setInstantTownHall(bool b){
        instantTownHall = b;
    }

    void setInstantBuilding(bool b){
        instantBuilding = b;
    }

    void setHarvestForever(bool b){
        harvestForever = b;
    }

    void setAutoAttack(bool b){
        autoAttack = b;
    }

    void setFoodLimit(int b){
        foodLimit = b;
    }

    void setFarm(bool b){
        farmEnabled = b;
    }

    void setBarracks(bool b){
        barracksEnabled = b;
    }

    void setFootman(bool b){
        footmanEnabled = b;
    }

    void setArcher(bool b){
        archerEnabled = b;
    }

    void setStartGold(int v) {
        startGold = v;
    }

    void setStartFood(int v) {
        startFood = v;
    }

    void setStartStone(int v){
        startStone = v;
    }

    void setStartLumber(int v){
        startLumber = v;
    }

    void setTerminalSignal(bool b){
        terminalSignal = b;
    }

    static Config defaults(){
        Config config;
        config.setTickModifier(10);
        config.setInstantTownHall(false);
        config.setInstantBuilding(true);
        config.setHarvestForever(true);
        config.setAutoAttack(true);
        config.setFoodLimit(100);
        config.setFarm(true);
        config.setBarracks(false);
        config.setFootman(true);
        config.setArcher(false);
        config.setConsoleCaptionEnabled(false);
        config.setStartGold(1500);
        config.setStartLumber(750);
        config.setStartStone(0);
        config.setTerminalSignal(true);
        config.setPOMDP(false);

        // Engine only
        config.setGUI("Blend2DGui");
        return config;
    }



};
