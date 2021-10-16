#ifndef BVH_H
#define BVH_H

#include "Common.h"
#include "Object3D.h"
#include "AABB.h"
#include "Group.h"

#include <memory>
#include <cassert>
#include <algorithm>


bool cmp(shared_ptr<Object3D> o1, shared_ptr<Object3D> o2, int axis)
{
    AABB box1, box2;
    assert (o1->boundingBox(box1) && o2->boundingBox(box2));
    return box1.minimum[axis] < box2.minimum[axis];
}

template <int Axis>
bool cmp_axis(shared_ptr<Object3D> o1,  shared_ptr<Object3D> o2){
    return cmp(o1,o2,Axis);
}

class BVH_node : public Group
{
public:
    BVH_node(){}

    BVH_node(Group& group, size_t start, size_t end)
    {
        assert(end - start > 0);
        int _size = end - start;
        int axis = rand() % 3;

        if (_size == 1){
            left = right = make_shared<Object3D>(group.get(start));
        }
        else if (_size == 2){
            if (cmp_axis<axis>(group.get(0), group.get(1))){
                left = group.get(0);
                right = group.get(1);
            }else{
                left = group.get(1);
                right = group.get(0);
            }
        }
        else{
            //sort alone axis
            std::sort(group._container.begin()+start, group._container.begin()+end, cmp<axis>);
            int mid = (end-start)/2 + start;
            left = make_shared<Group>(start, mid , group);
            right = make_shared<Group>(mid, end , group);
        }
       
    }
    
    bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override
    {
        if (! box.hit(ray, t_min, t_max))
            return false;
        return left->hit(ray, t_min, t_max, rec) || right->hit(ray, t_min, t_max, rec);
    }
    bool boundingBox(AABB& outputBox) const override
    {
        outputBox = box;
        return true;
    }

    shared_ptr<Object3D> left;
    shared_ptr<Object3D> right;
    AABB box;
};

#endif