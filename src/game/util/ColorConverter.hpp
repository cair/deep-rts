//
// Created by Per-Arne on 27.02.2017.
//

#ifndef WARC2SIM_COLORCONVERTER_H
#define WARC2SIM_COLORCONVERTER_H


#include <SFML/Graphics/Color.hpp>

class ColorConverter {


public:
    static sf::Color hsv(double hue, double sat, double val)
    {
        hue = fmod(hue, 360);
        while(hue<0) hue += 360;

        if(sat<0.f) sat = 0.f;
        if(sat>1.f) sat = 1.f;

        if(val<0.f) val = 0.f;
        if(val>1.f) val = 1.f;

        int h = hue/60;
        double f = (hue)/60-h;
        double p = val*(1.f-sat);
        double q = val*(1.f-sat*f);
        double t = val*(1.f-sat*(1-f));

        switch(h)
        {
            default:
            case 0:
            case 6: return sf::Color(val*255, t*255, p*255);
            case 1: return sf::Color(q*255, val*255, p*255);
            case 2: return sf::Color(p*255, val*255, t*255);
            case 3: return sf::Color(p*255, q*255, val*255);
            case 4: return sf::Color(t*255, p*255, val*255);
            case 5: return sf::Color(val*255, p*255, q*255);
        }
    }
};


#endif //WARC2SIM_COLORCONVERTER_H
