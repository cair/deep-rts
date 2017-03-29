#ifndef SFML_PLOT_AXIS_H
#define SFML_PLOT_AXIS_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace sf
{
namespace plot
{

class Axis
        : public sf::Drawable
        , public sf::Transformable
{
private:
    /// split the axis in kPart part !
    static const int kPart = 4;
public:
    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ////////////////////////////////////////////////////////////
    Axis();

    ////////////////////////////////////////////////////////////
    /// \brief set the size of the axis
    ///
    /// \param size new size
    ////////////////////////////////////////////////////////////
    void setSize(float size);

    ////////////////////////////////////////////////////////////
    /// \brief set the name of this axis
    ///
    /// \param name axis name
    ////////////////////////////////////////////////////////////
    void setName(const std::string &name);

    ////////////////////////////////////////////////////////////
    /// \brief prepare the axis to draw
    ///
    /// \param range min/max value for axis
    ////////////////////////////////////////////////////////////
    void prepare(const Vector2f &range);

    ////////////////////////////////////////////////////////////
    /// \brief give the font instance
    ///
    /// \param font pointer already load
    ////////////////////////////////////////////////////////////
    void setFont(sf::Font *font);
private:
    ////////////////////////////////////////////////////////////
    /// \brief setup the axis label
    ////////////////////////////////////////////////////////////
    void defineLabel();

    ////////////////////////////////////////////////////////////
    /// \brief sfml draw method
    ////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /// axis name
    std::string name_;

    /// global font
    sf::Font *font_;

    /// axis size
    float size_;

    /// axis label
    sf::Text label_;

    /// axis number
    std::vector<sf::Text> numbers_;
};

inline void Axis::setFont(sf::Font *font)
{
    font_ = font;
    defineLabel();
}

inline void Axis::setSize(float size)
{
    size_ = size;
}

inline void Axis::setName(const std::string &name)
{
    name_ = name;
    defineLabel();
}

}
}

#endif // SFML_PLOT_AXIS_H
