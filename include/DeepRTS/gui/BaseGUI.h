//
// Created by per on 9/5/21.
//

#ifndef DEEPRTS_BASEGUI_H
#define DEEPRTS_BASEGUI_H

#include <opencv2/core/mat.hpp>

class Game;
class Tile;


class BaseGUI{

protected:
    const Game& game;
public:
    cv::Mat renderData;
    explicit BaseGUI(Game& game);

    virtual const cv::Mat& render()const  = 0;
    virtual void onTileChange(const Tile&) = 0;
};


#endif //DEEPRTS_BASEGUI_H
