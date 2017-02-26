//
// Created by Per-Arne on 25.02.2017.
//

#ifndef WARC2SIM_SPRITEMANAGER_H
#define WARC2SIM_SPRITEMANAGER_H


#include <SFML/Graphics/Vertex.hpp>
#include <vector>
#include <map>

class Animation {
private:

    // Unit -> State -> sprite[]
    static std::map<int, std::map<int, std::vector<sf::Sprite>>> sprites;
public:
    static sf::Sprite & getNext(int unit, int state, int iteration);

    static void addSprite(const int unit, const int state, int x, int y, int w, int h, sf::Texture& texture);
    static void setup();
};


#endif //WARC2SIM_SPRITEMANAGER_H
