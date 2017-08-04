#include <limits>
#include "plot.h"

namespace sf
{
    namespace plot
    {

        Plot::Plot()
        {
        }

        Plot::Plot(const Vector2f &size, const std::string &title)
        {
            setTitle(title);
            setSize(size);
        }

        void Plot::setSize(const sf::Vector2f& size)
        {
            size_ = size;
            graphPos_.x = size_.x * (kLeftSize/100.0);
            graphPos_.y = size_.y * (kTopSize/100.0);

            graphSize_.x = size_.x - kBorderSize * 2
                           - size_.x * (kLeftSize/100.0)
                           - size_.x * (kRightSize/100.0);
            graphSize_.y = size_.y - kBorderSize * 2
                           - size_.y * (kTopSize/100.0)
                           - size_.y * (kBottomSize/100.0);

            grid_.setup(graphSize_);
            grid_.setPosition(graphPos_);

            xaxis_.setSize(graphSize_.x);
            yaxis_.setSize(graphSize_.y);

            xaxis_.setPosition(graphPos_.x, graphPos_.y+graphSize_.y+5);
            yaxis_.setPosition(graphPos_.x-20, graphPos_.y+graphSize_.y);
            yaxis_.setRotation(-90);

            bg_.setSize(sf::Vector2f(size_.x-kBorderSize*2, size_.y-kBorderSize*2));
            bg_.setFillColor(sf::Color(220, 220, 200));
            bg_.setOutlineColor(sf::Color(200, 0, 0));
            bg_.setOutlineThickness(kBorderSize);
            bg_.setPosition(sf::Vector2f(kBorderSize, kBorderSize));
        }

        bool Plot::haveCurve(const std::string &name) const
        {
            return curves_.find(name) != curves_.end();
        }

        Curve& Plot::getCurve(const std::string& name)
        {
            if(haveCurve(name) == false)
                throw std::string("Curve not found");
            return curves_[name];
        }

        Curve &Plot::createCurve(const std::string &name, const sf::Color& color)
        {
            curves_[name] = Curve(graphSize_, color);
            curves_[name].setLabel(name);
            curves_[name].setPosition(graphPos_);
            return curves_[name];
        }

        void Plot::draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            states.transform *= getTransform();

            target.draw(bg_, states);
            target.draw(grid_, states);
            target.draw(title_, states);
            target.draw(xaxis_, states);
            target.draw(yaxis_, states);

            for(std::map<std::string, Curve>::const_iterator it=curves_.begin();it!=curves_.end();++it)
                target.draw(it->second, states);
        }


        void Plot::reset() {
            for(auto &curve : curves_) {
                curve.second.reset();
            }
        }

        void Plot::prepare()
        {
            sf::Vector2f rangex;
            sf::Vector2f rangey;

            rangex.x = std::numeric_limits<float>::max();
            rangey.x = std::numeric_limits<float>::min();

            rangey.x = std::numeric_limits<float>::max();
            rangey.y = std::numeric_limits<float>::min();

            for(std::map<std::string, Curve>::iterator it=curves_.begin();it!=curves_.end();++it)
            {
                Curve &curve = it->second;
                curve.prepare(rangex, rangey);
            }

            xaxis_.prepare(rangex);
            yaxis_.prepare(rangey);
        }



        void Plot::setFont(const std::string &filename)
        {
            if (!font_.loadFromFile(filename))
            {
                throw ;
            }
            title_.setFont(font_);
            xaxis_.setFont(&font_);
            yaxis_.setFont(&font_);
            title_.setPosition(sf::Vector2f(-title_.getLocalBounds().width/2+size_.x/2, 0));
        }

        void Plot::setTitle(const std::string &title)
        {
            title_.setString(title);
            title_.setCharacterSize(12);
        }


    }
}
