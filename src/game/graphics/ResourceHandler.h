
#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "../loaders/MapLoader.h"

class ResourceHandler {

    sf::Texture tileset;

    ResourceHandler() {

    }


    void loadTilemapTexture();



};



inline void ResourceHandler::loadTilemapTexture() {

    tileset = MapLoader::loadTexture();

}

