#include <cmath>
#include <limits>

#include "curve.h"

namespace sf
{
    namespace plot
    {

        Curve::Curve()
                : color_(sf::Color::White)
                , size_(sf::Vector2f(100, 100))
                , fill_(true)
                , thickness_(2)
                , limit_(1000)
        {
        }

        Curve::Curve(const Vector2f &size, const sf::Color& color)
                : color_(color)
                , size_(size)
                , fill_(true)
                , thickness_(2)
                , limit_(1000)
        {
            setColor(color);
        }

        void Curve::addValue(float value)
        {
            data_.push_back(value);
            int size = data_.size();
            if(size > 1000)
            {
                data_.pop_front();
            }
        }

        void Curve::prepare(sf::Vector2f &rangex, sf::Vector2f &rangey)
        {
            rangex.x = 0;
            rangex.y = data_.size();

            if(data_.size() > 1)
            {
                if(rangey.x == std::numeric_limits<float>::max())
                {
                    rangey.x = data_.front();
                }
                if(rangey.y == std::numeric_limits<float>::min())
                {
                    rangey.y = data_.front();
                }

                auto max_value = *std::max_element(data_.begin(), data_.end());
                auto min_value = *std::min_element(data_.begin(), data_.end());
                // min
                if(min_value < rangey.x)
                {
                    rangey.x = min_value;
                }
                // max
                if(max_value > rangey.y)
                {
                    rangey.y = max_value;
                }


                line_.clear();
                area_.clear();
                float distance = fabs(rangey.y-rangey.x);
                if(distance == 0) distance = 1;

                int xoffset = size_.x / (data_.size());

                while(xoffset * data_.size() < (size_.x+xoffset))
                    xoffset++;

                int i = 0;
                float x = 0;
                for(std::list<float>::reverse_iterator it=data_.rbegin();it!=data_.rend();++it)
                {
                    x = size_.x - (i++ * xoffset);
                    float realy = (*it - rangey.x) / distance;
                    float y = size_.y - size_.y * realy;
                    x = (x < 0) ? 0 : x;

                    line_.add(sf::Vector2f(x, y), thickness_, color_);
                    area_.add(sf::Vector2f(x, y), fillColor_, size_.y);
                }
            }
        }

        void Curve::draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            states.transform *= getTransform();
            if(fill_)
                target.draw(area_, states);
            target.draw(line_, states);
        }

        void Curve::reset() {
            data_.clear();
        }

    }
}
