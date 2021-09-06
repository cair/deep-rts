//
// Created by per-arne on 02.04.17.
//

#ifndef DEEPRTS_RESOURCELOADER_H
#define DEEPRTS_RESOURCELOADER_H

#include <nlohmann/json.hpp>

/// The TilePropertyData class holds json data in memory.
/// The data stems from the json maps.
struct TilePropertyData{
    std::unordered_map<int, std::string> tileID2Type;
    std::unordered_map<std::string, nlohmann::json> tileData;

    nlohmann::json& getTileData(int tileID){
        auto &tileType = tileID2Type[tileID];
        auto &tData = tileData[tileType];
        return tData;
    }
};

/// The resourceLoader is primarily used to load maps.
class ResourceLoader {
public:
    /// json object of the map
    nlohmann::json mapJSON;

    /// json object of the tile metadata
    nlohmann::json tileJSON;


private:
    /// Get the filepath of the filename
    /// \param fileName
    /// \return fullpath
    static std::string getFilePath(const std::string& fileName);

    /// Boolean if the map is loaded
    bool mapLoaded = false;

    /// Boolean if the tiles metadata is loaded
    bool tilesLoaded = true;

    /// Fucntion to load the tiles metadata json
    void loadTileJSON();

    /// CONSTRUCTOR. Loads the TileJSON
    ResourceLoader(){
        loadTileJSON();
    }

public:
    /// The loadMapJSON function takes in a path to a file and loads the corresponding json
    /// \param map_file
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
