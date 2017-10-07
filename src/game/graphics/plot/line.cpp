#include <cmath>

#include "line.h"

namespace sf
{
namespace plot
{

Line::Line()
{
    previous_ = sf::Vector2f(-1, -1);
    vertices_.setPrimitiveType(sf::TrianglesStrip);
}

void Line::clear()
{
    vertices_.clear();
    previous_ = sf::Vector2f(-1, -1);
}

namespace
{
template <typename T>
inline static T length(const sf::Vector2<T>& v)
{
  return std::sqrt(v.x*v.x + v.y*v.y);
}
}

void Line::add(const sf::Vector2f& point,
                unsigned int thickness,
                const sf::Color& color)
{
    if(previous_.x != -1 && previous_.y != -1)
    {
        sf::Vector2f p1 = previous_;
        sf::Vector2f p2 = point;
        sf::Vector2f  v = p2 - p1;

        v /= length(v);  // make it a unit vector
        sf::Vector2f  vp(-v.y, v.x);  // compute the vector perpendicular to v

        sf::Vector2f temp;
        temp.x = p1.x + pwidth_/2 * vp.x;
        temp.y = p1.y + pwidth_/2 * vp.y;
        vertices_.append(sf::Vertex(temp, color_));

        temp.x = p1.x - pwidth_/2 * vp.x;
        temp.y = p1.y - pwidth_/2 * vp.y;
        vertices_.append(sf::Vertex(temp, color_));

        temp.x = p2.x + thickness/2 * vp.x;
        temp.y = p2.y + thickness/2 * vp.y;
        vertices_.append(sf::Vertex(temp, color));

        temp.x = p2.x - thickness/2 * vp.x;
        temp.y = p2.y - thickness/2 * vp.y;
        vertices_.append(sf::Vertex(temp, color));
    }
    previous_ = point;
    pwidth_ = thickness;
    color_ = color;
}

void Line::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(vertices_, states);
}

}
}
