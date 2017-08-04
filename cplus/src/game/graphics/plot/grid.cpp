#include "grid.h"

namespace sf
{
namespace plot
{

Grid::Grid()
{
}

void Grid::setup(const Vector2f& size)
{
    color_ = sf::Color(150, 150, 150);
    size_ = size;
    vertices_.clear();

    vertices_.setPrimitiveType(sf::Lines);
    float offset = size_.y / kPart;
    for(int i=1;i<kPart;i++)
    {
        vertices_.append(sf::Vertex(sf::Vector2f(0, i*offset), color_));
        vertices_.append(sf::Vertex(sf::Vector2f(size_.x, i*offset), color_));
    }

    offset = size_.x / kPart;
    for(int i=1;i<kPart;i++)
    {
        vertices_.append(sf::Vertex(sf::Vector2f(i*offset, 0), color_));
        vertices_.append(sf::Vertex(sf::Vector2f(i*offset, size_.y), color_));
    }

    border_.setSize(size_);
    border_.setFillColor(sf::Color(0, 0, 0, 0));
    border_.setOutlineColor(sf::Color(0, 0, 0));
    border_.setOutlineThickness(1);
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(vertices_, states);
    target.draw(border_, states);
}

}
}
