#ifndef SFML_PLOT_AREA_H
#define SFML_PLOT_AREA_H

#include <SFML/Graphics.hpp>
#include <vector>

namespace sf
{
namespace plot
{

class Area
        : public sf::Drawable
        , public sf::Transformable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ////////////////////////////////////////////////////////////
    Area();

    ////////////////////////////////////////////////////////////
    /// \brief add a new point
    ///
    /// \param point new point
    /// \param color
    /// \param y max size
    ////////////////////////////////////////////////////////////
    void add(const sf::Vector2f& point,
             const sf::Color& color=sf::Color(255, 255, 0),
             float size_y=100);

    ////////////////////////////////////////////////////////////
    /// \brief clear all points
    ////////////////////////////////////////////////////////////
    void clear();
private:
    ////////////////////////////////////////////////////////////
    /// \brief sfml method for drawing
    ////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /// vertices storage
    sf::VertexArray vertices_;
};

}
}

#endif // SFML_PLOT_AREA_H
