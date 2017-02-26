//
// Created by Per-Arne on 24.02.2017.
//

#ifndef WARC2SIM_GUI_H
#define WARC2SIM_GUI_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include "../environment/Tile.h"

class Game;
class GUI{
private:
    Game& game;

    sf::View povView;
    sf::View fullView;
    sf::View minimapView;

    void createFrame();
    sf::Texture gameFrameTexture;
    sf::Sprite gameFrame;
    sf::Font font;
    Tile* selectedTile = NULL;

public:
    GUI(Game &game);
    void caption();
    void render();
    void update();




    bool panLeft = false;
    bool panRight = false;
    bool panUp = false;
    bool panDown = false;

    bool toggleFrame = true;

    int velocity = 10;

    sf::RenderWindow  window;
    void drawTiles();

    sf::View* currentView;

    void drawUnits();

    int mouseClick(int mX, int mY);

    void leftClick(Tile &tile);

    void drawSelected();
    void rightClick(Tile &tile);

    Unit* selectedUnit = nullptr;
};


#endif //WARC2SIM_GUI_H
