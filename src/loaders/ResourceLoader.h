//
// Created by per-arne on 02.04.17.
//

#ifndef DEEPRTS_RESOURCELOADER_H
#define DEEPRTS_RESOURCELOADER_H

#include <nlohmann/json.hpp>

struct TilePropertyData{
    std::unordered_map<int, std::string> tileID2Type;
    std::unordered_map<std::string, nlohmann::json> tileData;

    nlohmann::json& getTileData(int tileID){
        auto &tileType = tileID2Type[tileID];
        auto &tData = tileData[tileType];
        return tData;
    }
};

class ResourceLoader {
public:
    nlohmann::json mapJSON;
    nlohmann::json tileJSON;


private:
    static std::string getFilePath(const std::string& fileName);
    bool mapLoaded = false;
    bool tilesLoaded = true;
    void loadTileJSON();
    ResourceLoader(){
        loadTileJSON();
    }

public:
    void loadMapJSON(const std::string& map_file);

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
    ResourceLoader(ResourceLoader const&)               = delete;
    void operator=(ResourceLoader const&)  = delete;

    TilePropertyData tileData;
    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status
    static ResourceLoader& getInstance()
    {
        static ResourceLoader    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    static TilePropertyData loadMAPJson(const nlohmann::json& tJSON);
};


#endif //DEEPRTS_RESOURCELOADER_H
