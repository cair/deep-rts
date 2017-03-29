#ifndef SFML_PLOT_CURVE_H
#define SFML_PLOT_CURVE_H

#include <vector>
#include <list>
#include <string>
#include <SFML/Graphics.hpp>
#include "line.h"
#include "area.h"

namespace sf
{
namespace plot
{

class Curve
        : public sf::Drawable
        , public sf::Transformable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ////////////////////////////////////////////////////////////
    Curve();

    ////////////////////////////////////////////////////////////
    /// \brief another constructor
    ////
    /// \parma size curve size
    /// \param color curve color
    ////////////////////////////////////////////////////////////
    Curve(const sf::Vector2f &size, const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief set the label
    ////////////////////////////////////////////////////////////
    void setLabel(const std::string &label);

    ////////////////////////////////////////////////////////////
    /// \brief set the color
    ////////////////////////////////////////////////////////////
    void setColor(const sf::Color &color);

    ////////////////////////////////////////////////////////////
    /// \brief add a new value for the curve
    ////////////////////////////////////////////////////////////
    void addValue(float value);

    ////////////////////////////////////////////////////////////
    /// \brief prepare the curve to be ready to draw
    ////////////////////////////////////////////////////////////
    void prepare(sf::Vector2f &rangex, sf::Vector2f &rangey);

    ////////////////////////////////////////////////////////////
    /// \brief plain curve or not
    ////////////////////////////////////////////////////////////
    void setFill(bool fill);

    ////////////////////////////////////////////////////////////
    /// \brief define the thickness of the curve
    ////////////////////////////////////////////////////////////
    void setThickness(float thickness);

    ////////////////////////////////////////////////////////////
    /// \brief determine the number of point have to draw
    ////////////////////////////////////////////////////////////
    void setLimit(unsigned int limit);


    void reset();
private:
    ////////////////////////////////////////////////////////////
    /// \brief sfml method
    ////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /// curve label
    std::string label_;

    /// curve color
    sf::Color color_;

    /// cuve area color
    sf::Color fillColor_;

    /// curve data
    std::list<float> data_;

    /// curve size
    sf::Vector2f size_;

    /// plain ?
    bool fill_;

    /// line object
    Line line_;

    /// area object
    Area area_;

    /// thickness size
    float thickness_;

    /// limit of data
    unsigned int limit_;
};

inline void Curve::setLimit(unsigned int limit)
{
    limit_ = limit;
}

inline void Curve::setThickness(float thickness)
{
    thickness_ = thickness;
}

inline void Curve::setFill(bool fill)
{
    fill_ = fill;
}

inline void Curve::setLabel(const std::string &label)
{
    label_ = label;
}

inline void Curve::setColor(const sf::Color &color)
{
    color_ = color;
    fillColor_ = color;
    fillColor_.a = 100;
}

}
}

#endif // SFML_PLOT_CURVE_H
