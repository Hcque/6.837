#ifndef AABB_H
#define AABB_H

#include "Common.h"
#include "Ray.h"
#include "Object3D.h"
#include <cassert>

class AABB
{
public:
    AABB(){}
    AABB(point3 _min, point3 _max):
        minimum(_min), maximum(_max)
    {

    }
    bool hit(const Ray& ray, double& t_min, double& t_max) const 
    {
        point3 Ro = ray.origin();
        vec3 Rd = ray.direction();
        double t0,t1;
        for (int a = 0; a < 3; a++){
            t0 = (minimum[a] - Ro[a]) / Rd[a];
            t1 = (maximum[a] - Ro[a]) / Rd[a];
            // if (t0 < t_min) t_min = t0;
            // if (t1 > t_max) t_max = t1;
            assert(t0 < t1);
            if (t_min >= t_max) {
                return false;
            }
        }
        return true;
    }

    point3 minimum;
    point3 maximum;

};


#endif