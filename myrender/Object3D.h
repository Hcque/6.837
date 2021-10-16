#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Common.h"
#include "Ray.h"
#include "AABB.h"

class Material;

struct hit_record{
    point3 p;
    double t;
    vec3 normal;
    double u, v;
    shared_ptr<Material> material;
    bool is_front;
};

class Object3D
{
public:
    virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool boundingBox(AABB& outputBox) const = 0;
};

    void get_u_v(double& u, double& v, const point3& p)
    {
        double phi = atan2(p[2], -p[0]) + PI;
        double thi = -1.0*acos(p[1]);
        u = phi/(2*PI);
        v = thi/PI;
    }

class Sphere: public Object3D
{
public:
    Sphere() : center(vec3(0,0,0)), radius(1.0){}

    Sphere(vec3 _center, double _rad, shared_ptr<Material> _m): 
        center(_center),
        radius(_rad),
        material(_m)
    {
    }

    bool hit(const Ray& ray, double t_min,double t_max, hit_record& rec) const 
    {
        vec3 ro_cen = ray.origin() - center;
        vec3 rd = ray.direction();
        double a = vec3::dot(rd,rd);
        double b = 2*vec3::dot(ro_cen, rd);
        double c = vec3::dot(ro_cen, ro_cen) - radius*radius;
        double discrim = b*b - 4*a*c;
        if (discrim < 0) return false;

        double t1 = ((-1.0f)*b - sqrt(discrim)) / (2.0*a);
        double t2 = ((-1.0f)*b + sqrt(discrim)) / (2.0*a);
        double t;
        if (t1 < t_max && t1 > t_min){
            t = t1;
        }else if (t2 < t_max && t2 > t_min){
            t = t2;
        }else{
            return false;
        }
        rec.t = t;
        rec.p = ray.at(t);
        rec.normal = (rec.p - center).normalize();
        rec.material = material;
        rec.is_front = vec3::dot(rec.normal, ray.direction()) < 0;
        get_u_v(rec.u, rec.v, rec.p);
        return true; 
    }

    bool boundingBox(AABB& outputBox) const 
    {
        point3 minimum, maximum;
        minimum = point3(center[0]-radius, center[1]-radius, center[2]-radius);
        maximum = point3(center[0]+radius, center[1]+radius, center[2]+radius);
        outputBox = AABB(minimum, maximum);
        return true;
    }

    vec3 center;
    double radius;
    shared_ptr<Material> material;
};

#endif