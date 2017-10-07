#include "area.h"

namespace sf
{
namespace plot
{

Area::Area()
{
    vertices_.setPrimitiveType(sf::TrianglesStrip);
}

void Area::clear()
{
    vertices_.clear();
}

void Area::add(const sf::Vector2f& point,
             const sf::Color& color,
               float size_y)
{
    vertices_.append(sf::Vertex(point, color));
    vertices_.append(sf::Vertex(sf::Vector2f(point.x, size_y), color));
}

void Area::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(vertices_, states);
}

}
}
