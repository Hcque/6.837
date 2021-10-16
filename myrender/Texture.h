#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec.h"
#include "Perlin.h"

class Texture
{
public:
    virtual color value(double u, double v, const point3 &p) const = 0;
};

class Solid: public Texture{
public:
    Solid(color _c): m_color(_c) {}

    color value(double u, double v, const point3 &p) const
    {
        return m_color;
    }

    color m_color;
};

class Checker : public Texture
{
public:
    Checker(color c_odd, color c_even): 
        odd(make_shared<Solid>(c_odd)),
        even(make_shared<Solid>(c_even))
    {

    }
    color value(double u, double v, const point3 &p) const{
        double scale = 10.0;
        double sinx = sin(p[0]*scale) * sin(p[1]*scale) * sin(p[2]*scale);
        if (sinx > 0){
            return odd->value(u,v,p);
        }
        else return even->value(u,v,p);
    }

    shared_ptr<Texture> odd;
    shared_ptr<Texture> even;
};

class Noise: public Texture{
public:
    Noise(){}
    Noise(double _scale=1.0): scale(_scale){}
    color value(double u, double v, const point3 &p) const{
        // return vec3(1,1,1) * m_noise.noise(scale*p);
        // return vec3(1,1,1) * m_noise.turb(scale*p);
        return vec3(1,1,1) * 0.5*(1+ sin( scale* p.z() + 10* m_noise.turb(scale*p)));

    }

    Perlin m_noise;
    double scale;
};

#endif