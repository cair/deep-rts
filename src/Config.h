//
// Created by per on 12.02.18.
//

#pragma once

class Config {
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
    bool gui = false;  // No need to export because it is used to initiate gui on C++ side only
    int startGold = 0;
    int startOil = 0;
    int startLumber = 0;
    int terminalSignal = true;
    /// Bool that determine print of FPS and UPS in console
    bool consoleCaptionEnabled = true;

    void setGUI(bool b){
        gui = b;
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

    void setStartOil(int v){
        startOil = v;
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
        config.setStartOil(0);
        config.setTerminalSignal(true);
        config.setPOMDP(false);

        // Engine only
        config.setGUI(false);
        return config;
    }



};
