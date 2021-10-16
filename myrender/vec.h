#ifndef VEC_H
#define VEC_H

#include <math.h>
#include <cassert>
#include <iostream>
#include <Eigen/Dense>
#include "Common.h"


class vec3
{
public:
    vec3(): e{0,0,0} {}
    vec3(double e1, double e2, double e3): e{e1,e2,e3} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator -() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v){
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const double t){
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    static double dot(const vec3 &u, const vec3 &v){
        return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
    }


    // friend inline std::ostream& operator<<(std::ostream& out, const vec3& v){
    //     return out << v.e[0] << " " <<v.e[1] << " " << v.e[2];
    // } 



    double length() const {
        return sqrt(dot(*this, *this));
    }
    void normalized(){
        double len = length();
        e[0]/= len;
        e[1]/= len;
        e[2]/= len;
    }
    vec3 normalize() const {
        double len = length();
        return vec3(e[0]/len, e[1]/len, e[2]/len);
    }

    double e[3];
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v){
    out << v.e[0] << " " << v.e[1] << " " << v.e[2];
    return out;
} 

inline vec3 operator+(const vec3 &u, const vec3 &v){
    return vec3(u[0]+v[0] , u[1]+v[1] , u[2]+v[2]);
}
inline vec3 operator-(const vec3 &u, const vec3 &v){
    return vec3(u[0]-v[0] , u[1]-v[1] , u[2]-v[2]);
}
inline vec3 operator*(const vec3 &u, const vec3 &v){
    return vec3(u[0]*v[0] , u[1]*v[1] , u[2]*v[2]);
}
inline vec3 operator*(const double &t, const vec3 &v){
    return vec3(t*v[0] , t*v[1] , t*v[2]);
}
inline vec3 operator*(const vec3 &v, const double &t){
    return vec3(t*v[0] , t*v[1] , t*v[2]);
}
inline vec3 operator/(const vec3 &v, const double &t){
    return (1/t)*v;
}

vec3 cross(const vec3& u, const vec3& v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);

}
typedef vec3 point3;
typedef vec3 color;

color randomColor(double low = 0, double high = 1){
    return color( 
        generateRandom(low,high),
        generateRandom(low,high),
        generateRandom(low,high)
    );
}

vec3 sampleOnBall(){
    vec3 r;
   do {
   double x = generateRandom(-1, 1); 
   double y = generateRandom(-1, 1); 
   double z = generateRandom(-1, 1); 
   r = vec3(x,y,z);
    }
   while (vec3::dot(r,r)  > 1);

   return r;
}

vec3 sampleOnHemsphere( const vec3 &normal){
    vec3 u = sampleOnBall();
    if (vec3::dot(u, normal) > 0) return u;
    else return -u;
}


void writecolor(std::ostream& out, const color& v)
{
    int ir = static_cast<int>( 255.5*  sqrt(v.x() ) );
    int ig = static_cast<int>( 255.5* sqrt(v.y() ) );
    int ib = static_cast<int>( 255.5* sqrt(v.z() ) );

    // int ir = static_cast<int>( 255.5*  (v.x() ) );
    // int ig = static_cast<int>( 255.5* (v.y() ) );
    // int ib = static_cast<int>( 255.5* (v.z() ) );
    // gamma correction

    out << ir << " " << ig << " " << ib << "\n";
}

vec3 Ei4to3(const Eigen::Vector4f& v){
    return vec3(v(0), v(1), v(2));
}

Eigen::Vector4f v3to4(const vec3& v, double d){
    return Eigen::Vector4f(v[0],v[1],v[2],d);
}

#endif