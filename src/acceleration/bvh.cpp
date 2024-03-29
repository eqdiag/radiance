#include "bvh.h"
#include "math/rand.h"

#include <functional>


radiance::acceleration::BVHNode::BVHNode(std::vector<std::shared_ptr<geometry::Hittable>>& objects)
    :BVHNode(objects,0,objects.size())
{
}

radiance::acceleration::BVHNode::BVHNode(std::vector<std::shared_ptr<geometry::Hittable>>& objects, size_t start, size_t end)
{
    size_t len = end - start;

    if(len == 1){
        left_node = objects[start];
        right_node = objects[start];
    }else if(len == 2){
        left_node = objects[start];
        right_node = objects[start + 1];
    }else{
        //Here is where to experiment with various splitting criteria
        int axis = math::randInt(0,2);

        auto splitter = std::bind(compareBoxMins,axis,std::placeholders::_1,std::placeholders::_2);
        std::sort(objects.begin() + start,objects.begin() + end,splitter);

        size_t middle = start + len/2;
        left_node = std::make_shared<BVHNode>(objects,start,middle);
        right_node = std::make_shared<BVHNode>(objects,middle,end);
    }

    geometry::AABB left_box,right_box{};
    left_node->getBoundingBox(left_box);
    right_node->getBoundingBox(right_box);
    _box = left_box.merge(right_box);
}

bool radiance::acceleration::BVHNode::trace(const math::Ray& ray, geometry::Hit& hit, float tmin, float tmax) const
{
    geometry::AABB left_box{};
    geometry::AABB right_box{};

    //Check the left child
    float t_min = tmin;
    float t_max = tmax;

    //Return if no hit
    if(!_box.isHit(ray,t_min,t_max)) return false; 

    geometry::Hit left_hit{};
    geometry::Hit right_hit{};

    //Check if left hit
    if(left_node->trace(ray,left_hit,t_min,t_max)){

        //If also right hit, take the min
        if(right_node->trace(ray,right_hit,t_min,t_max)){
            hit = left_hit.t < right_hit.t ? left_hit : right_hit;
        }else{
            hit = left_hit;
        }
    }else{ //Otherwise, we could only hit the right node

        //If so, use that hit
        if(right_node->trace(ray,right_hit,t_min,t_max)){
            hit = right_hit;
        }else{ //Otherwise, we didn't hit anything
            return false;
        }
    }

    return true;
}

bool radiance::acceleration::BVHNode::getBoundingBox(radiance::geometry::AABB &box) const
{
    box = _box;
    return true;
}

bool radiance::acceleration::compareBoxMins(int axis, std::shared_ptr<geometry::Hittable> lhs, std::shared_ptr<geometry::Hittable> rhs)
{
    geometry::AABB left_box,right_box{};
    lhs->getBoundingBox(left_box);
    rhs->getBoundingBox(right_box);
    return left_box.min[axis] < right_box.min[axis];
}
