#ifndef SFML_PLOT_PLOT_H
#define SFML_PLOT_PLOT_H

#include <map>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "axis.h"
#include "grid.h"
#include "curve.h"

namespace sf
{
namespace plot
{

class Plot
        : public sf::Drawable
        , public sf::Transformable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ////////////////////////////////////////////////////////////
    Plot();

    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ///
    /// \param size plot size
    /// \param title plot title
    ////////////////////////////////////////////////////////////
    Plot(const sf::Vector2f &size, const std::string &title);

    ////////////////////////////////////////////////////////////
    /// \brief create a new curve
    ///
    /// \param name curve name
    /// \param color curve color
    ///
    /// \return Curve return ref to the new curve
    ////////////////////////////////////////////////////////////
    Curve &createCurve(const std::string &name, const Color &color);

    ////////////////////////////////////////////////////////////
    /// \brief test if a curve name exist
    ///
    /// \param name curve name query
    ///
    /// \return bool true if exist
    ////////////////////////////////////////////////////////////
    bool haveCurve(const std::string &name) const;

    ////////////////////////////////////////////////////////////
    /// \brief get a cuve
    ///
    /// \param name curve name query
    ///
    /// \return a ref to the curve of ... an exception
    ////////////////////////////////////////////////////////////
    Curve& getCurve(const std::string& name);

    ////////////////////////////////////////////////////////////
    /// \brief prepare the plot to be draw
    ////////////////////////////////////////////////////////////
    void prepare();

    ////////////////////////////////////////////////////////////
    /// \brief set the name of the X label
    ///
    /// \param name x label name
    ////////////////////////////////////////////////////////////
    void setXLabel(const std::string &name);

    ////////////////////////////////////////////////////////////
    /// \brief set the name of the Y label
    ///
    /// \param name y label name
    ////////////////////////////////////////////////////////////
    void setYLabel(const std::string &name);

    ////////////////////////////////////////////////////////////
    /// \brief set the color of the background
    ///
    /// \param color background color
    ////////////////////////////////////////////////////////////
    void setBackgroundColor(const sf::Color &color);

    ////////////////////////////////////////////////////////////
    /// \brief set the color of the title
    ///
    /// \param color title color
    ////////////////////////////////////////////////////////////
    void setTitleColor(const sf::Color &color);

    ////////////////////////////////////////////////////////////
    /// \brief set the title of the plot
    ///
    /// \param title
    ////////////////////////////////////////////////////////////
    void setTitle(const std::string &title);

    ////////////////////////////////////////////////////////////
    /// \brief set the font of the plot
    ///
    /// \param filename path of the font
    ////////////////////////////////////////////////////////////
    void setFont(const std::string &filename);

    ////////////////////////////////////////////////////////////
    /// \brief set the size
    ////////////////////////////////////////////////////////////
    void setSize(const sf::Vector2f& size);

    void reset();

private:
    ////////////////////////////////////////////////////////////
    /// \brief sfml method to draw
    ////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;


    /// x axis
    Axis xaxis_;

    /// y axis
    Axis yaxis_;

    /// grid isntance
    plot::Grid grid_;

    /// size of the chart
    sf::Vector2f size_;

    /// collection of curves
    std::map<std::string, Curve> curves_;

    /// graph size (reduce size from size)
    sf::Vector2f graphSize_;
    /// graph real position
    sf::Vector2f graphPos_;

    static const int kTopSize = 10;
    static const int kLeftSize = 12;
    static const int kRightSize = 5;
    static const int kBottomSize = 12;
    static const int kBorderSize = 0;

    /// global font
    sf::Font font_;

    /// title
    sf::Text title_;

    /// background
    sf::RectangleShape bg_;
};


inline void Plot::setXLabel(const std::string &name)
{
    xaxis_.setName(name);
}

inline void Plot::setYLabel(const std::string &name)
{
    yaxis_.setName(name);
}

inline void Plot::setTitleColor(const sf::Color &color)
{
    title_.setFillColor(color);
}

inline void Plot::setBackgroundColor(const sf::Color &color)
{
    bg_.setFillColor(color);
}

}
}

#endif // SFML_PLOT_PLOT_H

