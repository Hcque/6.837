
#include "Object3D.h"
#include "vec.h"
#include "Texture.h"
#include "Material.h"
#include <cassert>

class Medium : public Object3D
{
public:
    Medium() {}
    Medium(shared_ptr<Object3D> _boundary , color _c, double _density):
    boundary(_boundary),
    iso(make_shared<Isotrophic>(_c)),
    density(_density)
    {}
    
    Medium(shared_ptr<Object3D> _boundary , shared_ptr<Material> _m, double _density): 
        boundary(_boundary),
        iso(_m),
        density(_density) 
        {

        }
    virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const 
    {
        hit_record rec1, rec2;
        double t1 = __DBL_MAX__;
        double t2 = __DBL_MAX__;
        if (! boundary->hit(ray, -t1, t1, rec1)){
            return false;
        }
        if (! boundary->hit(ray, rec1.t+0.01, t2, rec2)){
            return false;
        }
        assert(rec1.t <= rec2.t);

        double rand_dist = -1.0/density *log(1.0*rand()/RAND_MAX);
        double speed = ray.direction().length();
        if (rand_dist > (rec2.t-rec1.t ) *  speed)
            return false;
        
        rec.t = rec1.t + rand_dist/speed;
        rec.is_front = false;
        rec.normal = sampleOnBall().normalize();
        rec.p = ray.at(rec.t);
        rec.material = iso;
        return true;
    }
    virtual bool boundingBox(AABB& outputBox) const {
        return false;
    }

    shared_ptr<Object3D> boundary;
    shared_ptr<Material> iso;
    double density;

};
