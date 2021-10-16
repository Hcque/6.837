#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Ray.h"

class Camera
{
public:
    Camera() {}

protected:
    point3 origin;
    vec3 focal, up, horizontal;
    vec3 low_left_corner;

};

class PerspectiveCamera: public Camera
{
public: 
    PerspectiveCamera(){}
    PerspectiveCamera(point3 _lookfrom, point3 _lookat, vec3 _vup,
     double _angle, double _ratio, int _width = 400, double focal_length = 1):
        lookfrom(_lookfrom), lookat(_lookat), vup(_vup),
        angle(_angle), ratio(_ratio), image_width(_width)
    {
        // const int image_width = 400;
        const int image_height = static_cast<int>(1.0f*image_width/ratio);

        vec3 w = (lookfrom - lookat).normalize();
        vec3 u = cross(vup, w);
        vec3 v = cross(w, u);
        double angle_rad = (angle/ 360) *(2*PI);
        auto viewport_height = 2* tan(angle_rad/2.0);
        auto viewport_width = ratio*viewport_height;
        // auto focal_length = 1.0;

        origin = lookfrom;
        // horizontal = vec3(viewport_width, 0,0);
        horizontal = viewport_width * u;
        // up = vec3(0, viewport_height,0);
        up = viewport_height * v;
        focal = vec3(0,0,-focal_length);
        // focal = w ;
        // DEBUG(up);
        low_left_corner = 	origin + (-w) - horizontal/2 - up/2;
        // DEBUG(low_left_corner);

    }

    Ray generateRay(double x_ratio, double y_ratio)
    {
        vec3 dir = low_left_corner + x_ratio*horizontal + y_ratio*up - origin;
        dir.normalized();
        return Ray(origin, dir);
    }
protected:
    int image_width;
    int image_height;

    point3 lookfrom, lookat;
    vec3 vup;
    double angle;
    double ratio;
    double focal_length;

};

#endif
