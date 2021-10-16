#ifndef RAY_H
#define RAY_H

#include "vec.h"

class Ray
{
public:
    Ray(){}

    Ray(const vec3& origin,const vec3& direction):
        m_orgin(origin), m_direction(direction.normalize())
    {

    }

    point3 at(double t) const {
        return m_orgin + t*m_direction;
    }

    point3 origin() const {return m_orgin; }
    point3 direction() const {return m_direction; }

private:
    vec3 m_orgin;
    vec3 m_direction;
};

#endif