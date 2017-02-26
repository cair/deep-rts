//
// Created by Per-Arne on 25.02.2017.
//

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Animation.h"
#include "../Constants.h"

void Animation::addSprite(const int unit, const int state, int x, int y, int w, int h, sf::Texture& texture) {

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(x, y, w, h));



}

void Animation::setup() {
    sf::Texture peasant;
    peasant.loadFromFile("data/textures/human/peasant.png");


    Animation::addSprite(Constants::Unit_Peasant, Constants::State_Idle, 0, 0, 32, 32, peasant);

}

/*sf::Sprite &Animation::getNext(int unit, int state, int iteration) {

    std::vector<sf::Sprite> &items = Animation::sprites[unit][state];

    sf::Sprite &sprite = items[iteration % items.size()];
    return sprite;


}*/
