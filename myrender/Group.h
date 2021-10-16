#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include <cmath>
#include <float.h>
#include "Common.h"
#include "Object3D.h"
#include "Ray.h"
#include "AABB.h"

using std::shared_ptr;
using std::make_shared;

#define getmin(x, y) ((x<y)?x:y)
#define getmax(x, y) ((x>y)?x:y)
AABB surroundingBox(const AABB& b1,const AABB& b2)
{
    point3 minimum( 
        getmin(b1.minimum[0], b2.minimum[0]),
        getmin(b1.minimum[1], b2.minimum[1]),
        getmin(b1.minimum[2], b2.minimum[2])
                    );

    point3 maximum( 
        std::max(b1.maximum[0], b2.maximum[0]),
        std::max(b1.maximum[1], b2.maximum[1]),
        std::max(b1.maximum[2], b2.maximum[2])
                    );
    return AABB(minimum, maximum);
}

class Group : public Object3D
{
public:
    Group(){}
    Group(std::vector<shared_ptr<Object3D>> _c){
        for (auto i: _c){
            _container.push_back(i);
        }
    }
    Group(int start, int end, const Group& _g){
        for (int i = start; i < end; i++){
            _container.push_back(_g.get(i));
        }
    }
    void addGroup(shared_ptr<Object3D> item){
        _container.push_back(item);
    }
    shared_ptr<Object3D> get(int i) const{ return _container[i]; }

    int size() const { return _container.size();  }

    bool hit(const Ray& ray, double t_min,double t_max, hit_record& rec) const
    {   
        bool result = false;
        for (int i = 0; i < _container.size(); ++i){
            if (_container[i]->hit(ray, t_min, t_max, rec)) {
                t_max = rec.t;
                result = true; 
            }
        }
        return result;
    }

     bool boundingBox(AABB& outputBox) const
     {
         AABB tmpBox; //TODO
         bool res;
         if (this->size() == 0) return false;
         if (this->size() == 1){
            res = this->get(0)->boundingBox(tmpBox);
            outputBox = tmpBox;
            return res;
         } 
         for (int i = 1; i < this->size(); i++){
             AABB aabb;
              assert(_container[i]->boundingBox(aabb));
             tmpBox = surroundingBox(tmpBox,aabb);
         }
         return true;

     }

    std::vector<shared_ptr<Object3D>> _container;
};

#endif
