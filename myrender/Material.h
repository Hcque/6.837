#ifndef MATERIAL_H
#define MATERIAL_H

#include "Common.h"
#include "Ray.h"
#include "vec.h"
#include "Object3D.h"
#include "Texture.h"

using std::pow;
using std::sqrt;


vec3 reflect(const vec3& ray_in, const vec3& normal){
    return ray_in - vec3::dot(ray_in,normal)* 2 * normal;
}

bool refract(const vec3& dir_in, const vec3& normal, double et_etp, vec3& refract){
    // double cos_theta = std::min( vec3::dot(-1.0f*dir_in, normal), 1.0);
    // vec3 ray_out_perp = eta_over_etaprime *(dir_in + cos_theta*normal);
    // vec3 ray_out_parel = -1*sqrt( std::abs(1 - std::pow(ray_out_perp.length(), 2) )) * normal ;
    // return (ray_out_parel + ray_out_perp).normalize();
    double d_N = vec3::dot(dir_in, normal);
    double discrim = 1 - pow(et_etp,2)*(1- d_N*d_N);
    if (discrim < 0) {
        return false;
    }
    refract = (et_etp * (dir_in - normal*(d_N)) - normal * sqrt(discrim)).normalize();
    return true;
}



class Material
{
public: 
    virtual bool scatter(const Ray& ray_in, Ray& ray_out, const hit_record& rec, color &attenu) const = 0;
    virtual bool emission(color& e) const { return false; }

    vec3 diffuseColor;

};

class Emission : public Material{
public:
    Emission(color _c): m_emission(_c){}
    bool scatter(const Ray& ray_in, Ray& ray_out, const hit_record& rec, color &attenu) const 
    { return false; }
     bool emission(color& e) const override {
         e = m_emission;
         return true;
     }

    color m_emission;

};

class Lambientian : public Material
{
public:
    Lambientian(const color& c): attenuation(make_shared<Solid>(c) ) {}
    Lambientian(shared_ptr<Texture> _t): attenuation(_t) {}

    bool scatter(const Ray& ray_in, Ray& ray_out, const hit_record& rec, color &attenu) const override
    {
		point3 diffu_center = rec.p + rec.normal * 0.01 ; // for numeric
        vec3 diffu_dir = diffu_center + rec.normal + sampleOnHemsphere(rec.normal) ;
		diffu_dir.normalized();
		ray_out = Ray(diffu_center, diffu_dir);
        attenu = attenuation->value(rec.u, rec.v, rec.p);
        return true;
    }

    // color attenuation;
    shared_ptr<Texture> attenuation;

};


class Reflective : public Material
{
public:
    Reflective(const color& c, double _fuzz): 
        attenuation(c), fuzz(_fuzz)
    {}

    bool scatter(const Ray& ray_in, Ray& ray_out, const hit_record& rec, color &attenu) const override
    {
		point3 diffu_center = rec.p  ; // for numeric
        vec3 refle_dir = reflect(ray_in.direction().normalize(), rec.normal);
		refle_dir.normalized();
        refle_dir += fuzz * sampleOnBall();
        if (vec3::dot(refle_dir, rec.normal) <= 0) 
            return false;
		ray_out = Ray(diffu_center, refle_dir);
        attenu = attenuation;
        return true;
    }

    color attenuation;
    double fuzz;

};

class Snell : public Material
{
public:
    Snell(double _id):id(_id) {}

    bool scatter(const Ray& ray_in, Ray& ray_out, const hit_record& rec, color &attenu) const override
    {
		point3 diffu_center = rec.p ; // for numeric
        attenu = color(1,1,1);
        double ratio = (rec.is_front) ? (1.0f/id) : id;
        vec3 dir_in = ray_in.direction().normalize();

        vec3 dir_out;
        vec3 N;
        if (vec3::dot( rec.normal, dir_in) > 0 ){
            N = - rec.normal;
        }
        else N = rec.normal;

        if (! refract(dir_in, N, ratio, dir_out)) {
            // total reflection
            dir_out = reflect(dir_in, rec.normal);
            return true;
        }
        
        // DEBUG(dir_out );
        // dir_out = dir_in;
        ray_out = Ray(diffu_center, dir_out);
        return true;
    }
    double id;
};


class Isotrophic: public Material
{
public:
    Isotrophic(color _c) : albedo(make_shared<Solid>(_c)) {}
    Isotrophic(shared_ptr<Texture> _t) : albedo(_t) {}

    virtual bool scatter(const Ray& ray_in, Ray& ray_out, const hit_record& rec, color &attenu) const
    {

		point3 diffu_center = rec.p ; // for numeric
        vec3 dir_out = sampleOnBall();
        ray_out = Ray(diffu_center , dir_out);
        attenu = albedo->value(rec.u, rec.v, rec.p);
        return true;

    }

    shared_ptr<Texture> albedo;

};



#endif