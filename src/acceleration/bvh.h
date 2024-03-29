#pragma once

#include <vector>
#include <memory>

#include "geometry/aabb.h"
#include "geometry/hit.h"



namespace radiance{

    namespace acceleration{

        class BVHNode: public geometry::Hittable{

            public:
                BVHNode(std::vector<std::shared_ptr<radiance::geometry::Hittable>>& objects);
                BVHNode(std::vector<std::shared_ptr<radiance::geometry::Hittable>>& objects,size_t start,size_t end);

                bool trace(const math::Ray&,geometry::Hit&,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;
                bool getBoundingBox(radiance::geometry::AABB& box) const override;

            private:

                std::shared_ptr<geometry::Hittable> left_node{};
                std::shared_ptr<geometry::Hittable> right_node{};
                radiance::geometry::AABB _box{};
        };

        bool compareBoxMins(int axis,std::shared_ptr<geometry::Hittable> lhs,std::shared_ptr<geometry::Hittable> rhs);

    }
}

