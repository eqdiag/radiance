#pragma once



#include "geometry/aabb.h"
#include "geometry/hit.h"

#include <vector>
#include <memory>

namespace radiance{

    namespace acceleration{

        class BVHNode: public geometry::Hittable{

            public:
                BVHNode(geometry::HitList&& hitList);
                BVHNode(std::vector<std::shared_ptr<radiance::geometry::Hittable>>& objects);
                BVHNode(std::vector<std::shared_ptr<radiance::geometry::Hittable>>& objects,size_t start,size_t end);

                bool trace(const math::Ray& ray,geometry::Hit& hit,float tmin = 0.0f,float tmax = std::numeric_limits<float>::infinity()) const override;
                bool boundingBox(geometry::AABB& outBox) const override;
                
            private:

                std::shared_ptr<geometry::Hittable> left_node{};
                std::shared_ptr<geometry::Hittable> right_node{};
                radiance::geometry::AABB box{};
        };

        bool compareBoxMins(int axis,std::shared_ptr<geometry::Hittable> lhs,std::shared_ptr<geometry::Hittable> rhs);

    }
}
