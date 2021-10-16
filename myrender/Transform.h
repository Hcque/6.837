#include <Eigen/Dense>
#include "Object3D.h"
#include "AABB.h"
#include "Group.h"

inline Eigen::Vector4f v3tov4(vec3& v3, double d){
    return Eigen::Vector4f(v3[0],v3[1],v3[2],d);
}

class Transform: public Object3D
{
public:
    Transform(shared_ptr<Object3D> _obj, Eigen::Matrix4f _m):
        _object(_obj),
        _matrix(_m)
    {

    }
    virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const 
    {
        bool res;
        vec3 Ro = ray.origin();
        vec3 Rd = ray.direction();
        Eigen::Vector4f Ro4 = v3to4(Ro, 1);
        Eigen::Vector4f Rd4 = v3to4(Rd, 0);
        Ro4 = _matrix.inverse() * Ro4;
        Rd4 = _matrix.inverse() * Rd4;
        bool result = _object->hit(Ray(Ei4to3(Ro4), Ei4to3(Rd4)), t_min, t_max, rec);
        // change the normal
        if (res){
            rec.normal = Ei4to3( _matrix.inverse().transpose() * v3to4(rec.normal, 0) );
        }
        return res;
    }
    virtual bool boundingBox(AABB& outputBox) const 
    {
        AABB tmpBox;
        bool res = _object->boundingBox(tmpBox);
        if (res){
            Eigen::Vector4f _min = v3to4(tmpBox.minimum, 1);
            Eigen::Vector4f _max = v3to4(tmpBox.maximum, 1);
            _min = _matrix * _min;
            _max = _matrix * _max;

            outputBox = AABB( Ei4to3(_min), Ei4to3(_max) );
            return true;
        }
        return false;
    }

    Eigen::Matrix4f _matrix;
    shared_ptr<Object3D> _object;
};
