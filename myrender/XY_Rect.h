#ifndef XY_RECT_H
#define XY_RECT_H

#include "Object3D.h"
#include "Ray.h"
#include "Material.h"

// X = 0, Y = 1, Z = 2
template<int X, int Y>
class XY_Rect: public Object3D
{
public:
    XY_Rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<Material> _m):
        x0(_x0),x1(_x1),y0(_y0),y1(_y1), k(_k), material(_m)
    {

    }
    bool hit(const Ray& ray, double t_min,double t_max, hit_record& rec) const 
    {
        int Z = 3 - X - Y;
        double z0 = ray.origin()[Z];
        double zd = ray.direction()[Z];
        double t = (k - z0) / zd;
        
        point3 p = ray.at(t);
        if ( x0 <= p[X] && p[X] <= x1 &&
        y0 <= p[Y] && p[Y] <= y1 &&
        t > t_min && t < t_max)
        {
            // DEBUG(t);
            // std:: cerr << X << "|" << Y  << " |"<< t << "\n";
            rec.t = t;
            rec.p = p;
            rec.material = material;
            rec.normal = vec3(0,0,0);
            rec.normal[Z] = 1 ;
            if (vec3::dot(rec.normal, ray.direction()) > 0) rec.normal[Z] = -1;
            // DEBUG(rec.normal);
            rec.is_front = vec3::dot(rec.normal, ray.direction()) < 0;
            return true;
        }
        return false;

    }
    virtual bool boundingBox(AABB& outputBox) const {
        vec3 minimum, maximum;
        int Z = 3 - X-Y;
        minimum[X] = x0; maximum[X] = x1;
        minimum[Y] = y0; maximum[Y] = y1;
        minimum[Z] = k-0.001; maximum[Z] = k+0.001;
        outputBox = AABB(minimum, maximum);
        return true;
    }


    double x0,x1,y0,y1,k;
    shared_ptr<Material> material;


};

#endif