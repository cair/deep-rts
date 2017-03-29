#ifndef SFML_PLOT_LINE_H
#define SFML_PLOT_LINE_H

#include <SFML/Graphics.hpp>

namespace sf
{
namespace plot
{

class Line
        : public sf::Drawable
        , public sf::Transformable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ////////////////////////////////////////////////////////////
    Line();

    ////////////////////////////////////////////////////////////
    /// \brief add a new point
    ///
    /// \param thickness the width of this point
    /// \param color ...
    ////////////////////////////////////////////////////////////
    void add(const sf::Vector2f& point,
             unsigned int thickness=2,
             const sf::Color& color=sf::Color(255, 255, 0));

    ////////////////////////////////////////////////////////////
    /// \brief clear the path
    ////////////////////////////////////////////////////////////
    void clear();

    ////////////////////////////////////////////////////////////
    /// \brief return the size
    ///
    /// \param size_t path size
    ////////////////////////////////////////////////////////////
    size_t size() const;

    ////////////////////////////////////////////////////////////
    /// \brief draw the path
    ///
    /// \param sfml target render
    /// \param states
    ////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    /// list of the vertices
    sf::VertexArray vertices_;
    /// the previous point
    sf::Vector2f previous_;
    /// past thickness
    unsigned int pwidth_;
    /// past color
    sf::Color color_;
};

inline size_t Line::size() const
{
    return vertices_.getVertexCount();
}

}

}

#endif // SFML_PLOT_LINE_H
