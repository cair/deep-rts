#include <sstream>
#include <cmath>
#include <iomanip>

#include "axis.h"

namespace sf
{
namespace plot
{

Axis::Axis()
    : font_(0)
    , size_(0)
{
}

void Axis::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!font_) return ;
    states.transform *= getTransform();
    for(std::vector<sf::Text>::const_iterator it=numbers_.begin();it!=numbers_.end();++it)
        target.draw(*it, states);
    target.draw(label_, states);
}

void Axis::prepare(const Vector2f &range)
{
    if(!font_) return ;
    numbers_.clear();

    float distance = fabs(range.y-range.x);
    if(distance == 0) distance = 1;

    float decx = size_ / kPart;

    float offset = distance / kPart;
    for(int i=0;i<=kPart;i++)
    {
        sf::Text text;
        text.setFont(*font_);
        text.setCharacterSize(8);
        text.setFillColor(sf::Color::Black);
        std::stringstream ss;
        ss << std::setprecision(4) << range.x + offset * i;
        text.setString(ss.str());
        text.setPosition(i*decx-ss.str().size()*2, 0);
        numbers_.push_back(text);
    }
}

void Axis::defineLabel()
{
    if(!font_) return ;
    label_.setFont(*font_);
    label_.setCharacterSize(8);
    label_.setFillColor(sf::Color::Black);
    label_.setString(name_);

    if(getRotation() != 0)
        label_.setPosition(size_/2-name_.size()*5, -20);
    else
        label_.setPosition(size_/2-name_.size()*5, 20);
}

}
}
