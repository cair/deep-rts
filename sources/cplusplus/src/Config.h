//
// Created by per on 12.02.18.
//

#ifndef DEEPRTS_CONFIG_H
#define DEEPRTS_CONFIG_H


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
    bool enableAudio = false;
    int audioVolume = 0;
    bool pompd = false;
    bool fastgui = false;

    void setFastGUI(bool b){
        fastgui = b;
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

    void setAudio(bool b, int volume){
        enableAudio = b;
        audioVolume = volume;
    }


    static Config defaults(){
        Config config;
        config.setTickModifier(10);
        config.setInstantTownHall(true);
        config.setInstantBuilding(true);
        config.setHarvestForever(false);
        config.setAutoAttack(true);
        config.setFoodLimit(100);
        config.setFarm(true);
        config.setBarracks(true);
        config.setFootman(true);
        config.setArcher(false);
        config.setAudio(false, 0);
        return config;
    }



};


#endif //DEEPRTS_CONFIG_H
