//
// Created by per-arne on 02.04.17.
//

#ifndef DEEPRTS_RESOURCELOADER_H
#define DEEPRTS_RESOURCELOADER_H

#include "../../../include/rapidjson/include/rapidjson/document.h"

class ResourceLoader {
public:
    rapidjson::Document mapJSON;
    rapidjson::Document tileJSON;


private:
    static std::string getFilePath(const std::string& fileName);
    bool mapLoaded = false;
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
};


#endif //DEEPRTS_RESOURCELOADER_H
