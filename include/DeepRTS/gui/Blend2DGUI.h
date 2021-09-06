//
// Created by per on 9/4/21.
//

#ifndef DEEPRTS_GUI_H
#define DEEPRTS_GUI_H

#include <blend2d.h>
#include "gui/BaseGUI.h"
#include <cmrc/cmrc.hpp>
#include <utility>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "Constants.h"
CMRC_DECLARE(DeepRTSAssets);

using UnitStateData = std::unordered_map<Constants::State, std::vector<std::tuple<int, int, bool>>>;
using UnitData = std::unordered_map<Constants::Direction, UnitStateData>;
using Color = std::tuple<int, int, int>;


class Blend2DWrapper{
public:
    static BLImage subImage(const BLImage& src, int x, int y, int w, int h, BLFormat format=BL_FORMAT_PRGB32){
        BLImage img(w, h, format);
        BLContext ctx(img);
        ctx.setCompOp(BLCompOp::BL_COMP_OP_SRC_COPY);
        ctx.blitImage(BLPointI(0, 0), src, BLRectI(x, y, w, h));
        return img;
    }
    static void blit(int x, int y, BLContext& ctx, const BLImage& src){
        ctx.blitImage(BLPointI(x, y), src);
    }
};



class Blend2DUnitState{
    std::vector<BLImage> Spawning;
    std::vector<BLImage> Walking;
    std::vector<BLImage> Despawned;
    std::vector<BLImage> Harvesting;
    std::vector<BLImage> Building;
    std::vector<BLImage> Combat;
    std::vector<BLImage> Dead;
    std::vector<BLImage> Base;
    std::vector<BLImage> Idle;

    std::vector<BLImage>* getContainer(Constants::State state){
        std::vector<BLImage> *container;
        switch (state) {
            case Constants::Spawning:
                container = &Spawning;
                break;
            case Constants::Walking:
                container = &Walking;
                break;
            case Constants::Despawned:
                container = &Despawned;
                break;
            case Constants::Harvesting:
                container = &Harvesting;
                break;
            case Constants::Building:
                container = &Building;
                break;
            case Constants::Combat:
                container = &Combat;
                break;
            case Constants::Dead:
                container = &Dead;
                break;
            case Constants::Idle:
                container = &Idle;
                break;
            case Constants::Base:
                container = &Base;
                break;
        }
        return container;
    }

public:

    void load(const BLImage& src, int tileSize, int width, int height, const UnitStateData& stateData){
        for(const auto & pair: stateData){
            std::vector<BLImage> *container = getContainer(pair.first);
            for(const auto& frame: pair.second){
                auto [x, y, flip] = frame;
                container->push_back(Blend2DWrapper::subImage(src, x, y, tileSize * width, tileSize * height));
            }
        }
    }


    const BLImage& get(int animationCounter, Constants::State state) {
        std::vector<BLImage> *container = getContainer(state);
        auto idx = animationCounter % container->size();
        return container->at(idx);
    }
};
    
class BLend2DUnitDirection{
protected:
    Blend2DUnitState Up;
    Blend2DUnitState Left;
    Blend2DUnitState Down;
    Blend2DUnitState Right;
    Blend2DUnitState UpLeft;
    Blend2DUnitState UpRight;
    Blend2DUnitState DownLeft;
    Blend2DUnitState DownRight;

public:
    const BLImage& get(int animationCounter, Constants::Direction direction, Constants::State state) {
        switch(direction){
            case(Constants::Direction::Down):
                return Down.get(animationCounter, state);
            case Constants::Up:
                return Up.get(animationCounter, state);
            case Constants::Left:
                return Left.get(animationCounter, state);;
            case Constants::Right:
                return Right.get(animationCounter, state);
            case Constants::DownLeft:
                return DownLeft.get(animationCounter, state);
            case Constants::DownRight:
                return DownRight.get(animationCounter, state);
            case Constants::UpLeft:
                return UpLeft.get(animationCounter, state);
            case Constants::UpRight:
                return UpRight.get(animationCounter, state);
        }

        throw std::runtime_error("Should not come here.");
    }
};

class Blend2DUnit: public BLend2DUnitDirection{
    const std::string asset_path;
    const int tileSize;
    const int width;
    const int height;
    const UnitData data;
    const Color color;
    BLImage image;


protected:
    explicit Blend2DUnit(const Color& color, int tileSize, int width, int height, std::string path, UnitData data)
    : asset_path(std::move(path))
    , tileSize(tileSize)
    , width(width)
    , height(height)
    , data(std::move(data))
    {
        auto [r, g, b] = color;
        auto fs = cmrc::DeepRTSAssets::get_filesystem();
        auto tileData = fs.open(asset_path);
        cv::Mat rawData( 1, tileData.size(), CV_8UC1, (void*)tileData.begin() );
        cv::Mat decodedImage  =  cv::imdecode(rawData, cv::ImreadModes::IMREAD_UNCHANGED);
        cv::Mat mask;
        cv::inRange(decodedImage, cv::Scalar(150, 150, 150, 150), cv::Scalar(255, 255, 255, 255), mask);
        decodedImage.setTo(cv::Scalar(r, g, b, 255), mask);

        std::vector<uchar> buf;
        cv::imencode(".png", decodedImage, buf, std::vector<int>() );
//        imageBuf = (unsigned char *) realloc(imageBuf, buf.size());
//        memcpy(imageBuf, &buf[0], buf.size());
//
//        cv::imshow("lo", decodedImage);
//        cv::waitKey(1000);
        if ( decodedImage.data == NULL )
        {
            // Error reading raw image data
        }

        auto err = image.readFromData(buf.data(), buf.size());

        if (err) {
            throw std::runtime_error("Could not load the tileset texture");
        }

        load();

    }

    void load(){
        for(auto& pair: data){
            switch(pair.first){
                case(Constants::Direction::Down):
                    Down.load(image, tileSize, width, height, pair.second);
                    break;
                case Constants::Up:
                    Up.load(image, tileSize, width, height, pair.second);
                    break;
                case Constants::Left:
                    Left.load(image, tileSize, width, height, pair.second);
                    break;
                case Constants::Right:
                    Right.load(image, tileSize, width, height, pair.second);
                    break;
                case Constants::DownLeft:
                    DownLeft.load(image, tileSize, width, height, pair.second);
                    break;
                case Constants::DownRight:
                    DownRight.load(image, tileSize, width, height, pair.second);
                    break;
                case Constants::UpLeft:
                    UpLeft.load(image, tileSize, width, height, pair.second);
                    break;
                case Constants::UpRight:
                    UpRight.load(image, tileSize, width, height, pair.second);
                    break;
            }
        }



    }

};




class Blend2DFarm: public Blend2DUnit{
public:
    Blend2DFarm(const Color& color, int tileSize, int width, int height)
            : Blend2DUnit(color,
            tileSize, width, height,
            "assets/textures/farm.png",
            {

                    {
                            Constants::Direction::Up, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}}, // TODO
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::Down, {
                                                              {Constants::State::Spawning, {{32, 0, false}}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {{32, 0, false}}},
                                                              {Constants::State::Idle, {{0, 0, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::Right, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::Left, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::UpRight, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::UpLeft, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::DownLeft, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::DownRight, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    }
            })
    {}

};

class Blend2DPeon: public Blend2DUnit{

};
class Blend2DPeasant: public Blend2DUnit{

public:
    Blend2DPeasant(const Color& color, int tileSize, int width, int height)
    : Blend2DUnit(color,
                  tileSize, width, height,
            "assets/textures/peasant.png",
            {
                {
                             Constants::Direction::Up, {
                             {Constants::State::Spawning, {}},
                             {Constants::State::Walking, {{0, 0, false}, {32, 0, false}, {64, 0, false}, {96, 0, false},
                                                          {128, 0, false}}},
                             {Constants::State::Despawned, {}},
                             {Constants::State::Harvesting, {{0, 288, false}, {32, 288, false}, {64, 288, false}, {96, 288, false},
                                                             {128, 288, false}}},
                             {Constants::State::Building, {{0, 288, false}, {32, 288, false}, {64, 288, false}, {96, 288, false},
                                                           {128, 288, false}}},
                             {Constants::State::Combat, {{0, 288, false}, {32, 288, false}, {64, 288, false}, {96, 288, false},
                                                         {128, 288, false}}},
                             {Constants::State::Dead, {{164, 4, false}}}, // TODO
                             {Constants::State::Idle, {{0, 0, false}}}
                     }
                     },
                     {
                             Constants::Direction::Down, {
                             {Constants::State::Spawning, {}},
                             {Constants::State::Walking, {{0, 32, false}, {32, 32, false}, {64, 32, false}, {96, 32, false},
                                                          {128, 32, false}}},
                             {Constants::State::Despawned, {}},
                             {Constants::State::Harvesting, {{0, 352, false}, {32, 352, false}, {64, 352, false}, {96, 352, false},
                                                             {128, 352, false}}},
                             {Constants::State::Building, {{0, 352, false}, {32, 352, false}, {64, 352, false}, {96, 352, false},
                                                           {128, 352, false}}},
                             {Constants::State::Combat, {{0, 352, false}, {32, 352, false}, {64, 352, false}, {96, 352, false},
                                                         {128, 352, false}}},
                             {Constants::State::Dead, {{164, 4, false}}},
                             {Constants::State::Idle, {{0, 32, false}}}
                     }
                     },
                     {
                             Constants::Direction::Right, {
                             {Constants::State::Spawning, {}},
                             {Constants::State::Walking, {{0, 64, false}, {32, 64, false}, {64, 64, false}, {96, 64, false},
                                                          {128, 64, false}}},
                             {Constants::State::Despawned, {}},
                             {Constants::State::Harvesting, {{0, 320, false}, {32, 320, false}, {64, 320, false}, {96, 320, false}}},
                             {Constants::State::Building, {{0, 320, false}, {32, 320, false}, {64, 320, false}, {96, 320, false},
                                                           {128, 320, false}}},
                             {Constants::State::Combat, {{0, 320, false}, {32, 320, false}, {64, 320, false}, {96, 320, false}}},
                             {Constants::State::Dead, {{164, 4, false}}},
                             {Constants::State::Idle, {{0, 64, false}}}
                     }
                     },
                     {
                             Constants::Direction::Left, {
                             {Constants::State::Spawning, {}},
                             {Constants::State::Walking, {{0, 96, false}, {32, 96, false}, {64, 96, false}, {96, 96, false},
                                                          {128, 96, false}}},
                             {Constants::State::Despawned, {}},
                             {Constants::State::Harvesting, {{0, 320, true}, {32, 320, true}, {64, 320, true}, {96, 320, true}}},
                             {Constants::State::Building, {{0, 320, true}, {32, 320, true}, {64, 320, true}, {96, 320, true},
                                                           {128, 320, true}}},
                             {Constants::State::Combat, {{0, 320, true}, {32, 320, true}, {64, 320, true}, {96, 320, true}}},
                             {Constants::State::Dead, {{164, 4, false}}},
                             {Constants::State::Idle, {{0, 96, false}}}
                     }
                     },
                     {
                             Constants::Direction::UpRight, {
                             {Constants::State::Spawning, {}},
                             {Constants::State::Walking, {{0, 128, false}, {32, 128, false}, {64, 128, false}, {96, 128, false},
                                                          {128, 128, false}}},
                             {Constants::State::Despawned, {}},
                             {Constants::State::Harvesting, {{0, 256, false}, {32, 256, false}, {64, 256, false}}},
                             {Constants::State::Building, {{0, 256, false}, {32, 256, false}, {64, 256, false}}},
                             {Constants::State::Combat, {{0, 256, false}, {32, 256, false}, {64, 256, false}}},
                             {Constants::State::Dead, {{164, 4, false}}},
                             {Constants::State::Idle, {{0, 128, false}}}
                     }
                     },
                     {
                             Constants::Direction::UpLeft, {
                             {Constants::State::Spawning, {}},
                             {Constants::State::Walking, {{0, 160, false}, {32, 160, false}, {64, 160, false}, {96, 160, false},
                                                          {128, 160, false}}},
                             {Constants::State::Despawned, {}},
                             {Constants::State::Harvesting, {{0, 256, true}, {32, 256, true}, {64, 256, true}}},
                             {Constants::State::Building, {{0, 256, true}, {32, 256, true}, {64, 256, true}}},
                             {Constants::State::Combat, {{0, 256, true}, {32, 256, true}, {64, 256, true}}},
                             {Constants::State::Dead, {{164, 4, false}}},
                             {Constants::State::Idle, {{0, 160, false}}}
                     }
                     },
                     {
                             Constants::Direction::DownLeft, {
                             {Constants::State::Spawning, {}},
                             {Constants::State::Walking, {{0, 224, false}, {32, 224, false}, {64, 224, false}, {96, 224, false},
                                                          {128, 224, false}}},
                             {Constants::State::Despawned, {}},
                             {Constants::State::Harvesting, {{32, 320, true}, {64, 352, true}, {96, 352, true}, {128, 352, true}}},
                             {Constants::State::Building, {{0, 320, true}, {32, 320, true}}},
                             {Constants::State::Combat, {{32, 320, true}, {64, 352, true}, {96, 352, true}, {128, 352, true}}},
                             {Constants::State::Dead, {{164, 4, false}}},
                             {Constants::State::Idle, {{0, 224, false}}}
                     }
                     },
                     {
                             Constants::Direction::DownRight, {
                             {Constants::State::Spawning, {}},
                             {Constants::State::Walking, {{0, 192, false}, {32, 192, false}, {64, 192, false}, {96, 192, false},
                                                          {128, 192, false}}},
                             {Constants::State::Despawned, {}},
                             {Constants::State::Harvesting, {{32, 320, false}, {64, 352, false}, {96, 352, false},
                                                             {128, 352, false}}},
                             {Constants::State::Building, {{0, 320, false}, {32, 320, false}}},
                             {Constants::State::Combat, {{32, 320, false}, {64, 352, false}, {96, 352, false}, {128, 352, false}}},
                             {Constants::State::Dead, {{164, 4, false}}},
                             {Constants::State::Idle, {{0, 192, false}}}
                     }
                     }
            })
    {}
    


};
class Blend2DFootman: public Blend2DUnit{
public:
    Blend2DFootman(const Color& color, int tileSize, int width, int height)
            : Blend2DUnit(color,
                          tileSize, width, height,
            "assets/textures/footman.png",
            {

                    {
                            Constants::Direction::Up, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 0, false}, {32, 0, false}, {64, 0, false}, {96, 0, false},
                                                                                                  {128, 0, false}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 160, false}, {32, 160, false}, {64, 160, false}, {96, 160, false}}},
                                                              {Constants::State::Dead, {{164, 4, false}}}, // TODO
                                                              {Constants::State::Idle, {{0, 0, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::Down, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 128, false}, {32, 128, false}, {64, 128, false}, {96, 128, false},
                                                                                                  {128, 128, false}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 288, false}, {32, 288, false}, {64, 288, false}, {96, 288, false}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 128, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::Right, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 64, false}, {32, 64, false}, {64, 64, false}, {96, 64, false},
                                                                                                  {128, 64, false}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 224, false}, {32, 224, false}, {64, 224, false}, {96, 224, false}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 64, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::Left, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 64, true}, {32, 64, true}, {64, 64, true}, {96, 64, true},
                                                                                                  {128, 64, true}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 224, true}, {32, 224, true}, {64, 224, true}, {96, 224, true}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 64, true}}}
                                                      }
                    },
                    {
                            Constants::Direction::UpRight, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 32, false}, {32, 32, false}, {64, 32, false}, {96, 32, false},
                                                                                                  {128, 32, false}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 192, false}, {32, 192, false}, {64, 192, false}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 32, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::UpLeft, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 32, true}, {32, 32, true}, {64, 32, true}, {96, 32, true},
                                                                                                  {128, 32, true}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 192, true}, {32, 192, true}, {64, 192, true}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 32, true}}}
                                                      }
                    },
                    {
                            Constants::Direction::DownRight, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 96, false}, {32, 96, false}, {64, 96, false}, {96, 96, false},
                                                                                                  {128, 96, false}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Combat, {{0, 256, false}, {32, 256, false}, {64, 256, false}, {96, 256, false}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 96, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::DownLeft, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 96, true}, {32, 96, true}, {64, 96, true}, {96, 96, true},
                                                                                                  {128, 96, true}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 256, true}, {32, 256, true}, {64, 256, true}, {96, 256, true}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 96, true}}}
                                                      }
                    }
            })
    {}

};

class Blend2DBarracks: public Blend2DUnit{
public:
    Blend2DBarracks(const Color& color, int tileSize, int width, int height)
            : Blend2DUnit(color,
                          tileSize, width, height,
            "assets/textures/barracks.png",
            {

                    {
                            Constants::Direction::Up, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}}, // TODO
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::Down, {
                                                              {Constants::State::Spawning, {{96, 0, false}}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {{0, 0, false}}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {{96, 0, false}}},
                                                              {Constants::State::Idle, {{0, 0, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::Right, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::Left, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::UpRight, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::UpLeft, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::DownLeft, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::DownRight, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    }
            })
    {}

};

class Blend2DArcher: public Blend2DUnit{
public:
    Blend2DArcher(const Color& color, int tileSize, int width, int height)
            : Blend2DUnit(color,
                          tileSize, width, height,
            "assets/textures/archer.png",
            {

                    {
                            Constants::Direction::Up, {
                                                              {Constants::State::Spawning, {{0, 32, false}, {32, 32, false}, {64, 32, false}, {96, 32, false}}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 192, false}, {32, 192, false}}},
                                                              {Constants::State::Dead, {{164, 4, false}}}, // TODO
                                                              {Constants::State::Idle, {{0, 32, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::Down, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 160, false}, {32, 160, false}, {64, 160, false}, {96, 160, false},
                                                                                                  {128, 160, false}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 320, false}, {32, 320, false}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 160, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::Right, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 96, false}, {32, 96, false}, {64, 96, false}, {96, 96, false}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 256, false}, {32, 256, false}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 96, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::Left, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 96, true}, {32, 96, true}, {64, 96, true}, {96, 96, true}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 256, true}, {32, 256, true}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 96, true}}}
                                                      }
                    },
                    {
                            Constants::Direction::UpRight, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 32, false}, {32, 32, false}, {64, 32, false}, {96, 32, false},
                                                                                                  {128, 32, false}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 224, false}, {32, 224, false}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 32, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::UpLeft, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 32, true}, {32, 32, true}, {64, 32, true}, {96, 32, true},
                                                                                                  {128, 32, true}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 224, true}, {32, 224, true}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 32, true}}}
                                                      }
                    },
                    {
                            Constants::Direction::DownLeft, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 128, true}, {32, 128, true}, {64, 128, true}, {96, 128, true},
                                                                                                  {128, 128, true}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Combat, {{0, 288, true}, {32, 288, true}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 128, true}}}
                                                      }
                    },
                    {
                            Constants::Direction::DownRight, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {{0, 128, false}, {32, 128, false}, {64, 128, false}, {96, 128, false},
                                                                                                  {128, 128, false}}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {{0, 288, false}, {32, 288, false}}},
                                                              {Constants::State::Dead, {{164, 4, false}}},
                                                              {Constants::State::Idle, {{0, 128, false}}}
                                                      }
                    }
            })
    {}

};

class Blend2DTownHall: public Blend2DUnit{
public:
    Blend2DTownHall(const Color& color, int tileSize, int width, int height)
            : Blend2DUnit(color,
                          tileSize, width, height,
            "assets/textures/town_hall.png",
            {

                    {
                            Constants::Direction::Up, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}}, // TODO
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::Down, {
                                                              {Constants::State::Spawning, {{96, 0, false}}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {{0, 0, false}}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {{96, 0, false}}},
                                                              {Constants::State::Idle, {{0, 0, false}}}
                                                      }
                    },
                    {
                            Constants::Direction::Right, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::Left, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::UpRight, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::UpLeft, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::DownLeft, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    },
                    {
                            Constants::Direction::DownRight, {
                                                              {Constants::State::Spawning, {}},
                                                              {Constants::State::Walking, {}},
                                                              {Constants::State::Despawned, {}},
                                                              {Constants::State::Harvesting, {}},
                                                              {Constants::State::Building, {}},
                                                              {Constants::State::Combat, {}},
                                                              {Constants::State::Dead, {}},
                                                              {Constants::State::Idle, {}}
                                                      }
                    }
            })
    {}

};







class Blend2DTile{

public:


    explicit Blend2DTile(BLImage tile)
            : tile(std::move(tile))
    {
    }

    BLImage tile;
};


class Blend2DTileset{

    std::vector<Blend2DTile> tiles;
    const int tileWidth;
    const int tileHeight;

public:
    explicit Blend2DTileset(int tileWidth, int tileHeight, const std::string& srcPath)
            : tileWidth(tileWidth)
            , tileHeight(tileHeight)
    {
        BLImage tileset;
        auto fs = cmrc::DeepRTSAssets::get_filesystem();
        auto tileData = fs.open(srcPath);
        auto err = tileset.readFromData(tileData.begin(), tileData.size());
        if (err) {
            throw std::runtime_error("Could not load the tileset texture");
        }

        BLImageCodec codec;
        codec.findByName("BMP");

        tileset.writeToFile("lol.bmp", codec);

        load(tileset);
    }

    void load(BLImage& tileset){
        auto n_cols = tileset.width() / tileWidth;
        auto n_rows = tileset.height() / tileHeight;
        auto total_items = n_cols * n_rows;
        for(int i = 0; i < total_items; i++){
            auto x = i % n_cols;    // % is the "modulo operator", the remainder of i / width;
            auto y = i / n_cols;    // where "/" is an integer division
            auto x_start = (x * tileWidth) + x;
            auto y_start = (y * tileHeight) + y;

            tiles.emplace_back(Blend2DWrapper::subImage(tileset, x_start, y_start, tileWidth, tileHeight));
//            std::cout << "x=" << x_start << ",y=" << y_start << std::endl;
            //            tile.writeToFile((std::to_string(x) + "_" + std::to_string(y) + ".bmp").c_str());
        }

//        for(int i =0; i < tiles.size(); i++){
//            auto& tile = tiles[i].tile;
//            tile.writeToFile((std::to_string(i)  + ".bmp").c_str());
//
//        }
    }

    [[nodiscard]] const BLImage &at(int index) const{
        return tiles[index].tile;
    }
};



class Blend2DGUI: public BaseGUI{
    int tileSize = 32;
    BLImage canvas;
    BLContext ctx;
    BLImageData imageData;
    Blend2DTileset tileset;
    std::vector<std::unordered_map<Constants::Unit, Blend2DUnit>> units;

    const cv::Mat& render()const override;
    void onTileChange(const Tile& tile) override;

public:
    explicit Blend2DGUI(Game& game);

    void renderMap();

};


#endif //DEEPRTS_GUI_H
