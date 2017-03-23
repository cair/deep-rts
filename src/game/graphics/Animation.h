//
// Created by Per-Arne on 25.02.2017.
//

#ifndef WARC2SIM_SPRITEMANAGER_H
#define WARC2SIM_SPRITEMANAGER_H


#include <SFML/Graphics/Sprite.hpp>
#include <vector>
#include <map>
#include "../unit/Unit.h"


class Animation
{
public:
    static Animation& getInstance()
    {
        static Animation    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    // Unit -> State -> Direction ->sprite[]
    std::map<
            int, std::map<int,
                    std::map<int, std::vector<std::shared_ptr<sf::Sprite>>
                    >>> sprites;
    sf::Sprite & getNext(Unit & unit);

    void add(const int unit, const int state,  const int direction,int x, int y, int w, int h, int wrs, int hrs,  sf::Texture* texture, bool flip);
    void setup();

private:
    std::shared_ptr<sf::Texture> peasant;
    std::shared_ptr<sf::Texture> buildings;


    Animation() {}                    // Constructor? (the {} brackets) are needed here.

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
    Animation(Animation const&)               = delete;
    void operator=(Animation const&)  = delete;

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status
    void setupPeasant();

    void setupTownHall();


};









#endif //WARC2SIM_SPRITEMANAGER_H
