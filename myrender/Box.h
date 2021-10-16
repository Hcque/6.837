#include "Object3D.h"
#include "Group.h"
#include "XY_Rect.h"

#define X 0
#define Y 1
#define Z 2

class Box: public Object3D
{
public:

    Box(point3 p1, point3 p2, shared_ptr<Material> _m) 
    {
        face_list.addGroup( make_shared<XY_Rect<X,Y>>(p1.x(), p2.x(), p1.y(), p2.y(), p1.z(), _m) );
        face_list.addGroup( make_shared<XY_Rect<X,Y>>(p1.x(), p2.x(), p1.y(), p2.y(), p2.z(), _m) );
        face_list.addGroup( make_shared<XY_Rect<X,Z>>(p1.x(), p2.x(), p1.z(), p2.z(), p1.y(), _m) );
        face_list.addGroup( make_shared<XY_Rect<X,Z>>(p1.x(), p2.x(), p1.z(), p2.z(), p2.y(), _m) );
        face_list.addGroup( make_shared<XY_Rect<Y,Z>>(p1.y(), p2.y(), p1.z(), p2.z(), p1.x(), _m) );
        face_list.addGroup( make_shared<XY_Rect<Y,Z>>(p1.y(), p2.y(), p1.z(), p2.z(), p2.x(), _m) );
    }

    virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const 
    {
        return face_list.hit(ray, t_min, t_max, rec);
    }
    virtual bool boundingBox(AABB& outputBox) const 
    {
        return face_list.boundingBox(outputBox);
    }

    Group face_list;
};
