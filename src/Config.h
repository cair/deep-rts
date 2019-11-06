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
    bool enableFarm = true;
    bool enableBarracks = true;
    bool enableFootman = true;
    bool enableArcher = false;
    bool pompd = false;
    bool gui = false;
    int startGold = 0;
    int startOil = 0;
    int startWood = 0;
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
        enableFarm = b;
    }

    void setBarracks(bool b){
        enableBarracks = b;
    }

    void setFootman(bool b){
        enableFootman = b;
    }

    void setArcher(bool b){
        enableArcher = b;
    }

    void setStartGold(int v) {
        startGold = v;
    }

    void setStartOil(int v){
        startOil = v;
    }

    void setStartWood(int v){
        startWood = v;
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
        config.setStartWood(750);
        config.setStartOil(0);
        config.setTerminalSignal(true);
        config.setGUI(false);
        return config;
    }



};
