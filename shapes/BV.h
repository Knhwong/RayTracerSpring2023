#ifndef BV_H_
#define BV_H_

#include "math/geometry.h"
// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-acceleration-structure/bounding-volume-hierarchy-BVH-part1.html
// We use a three slab approach
namespace rt
{
    class BV
    {
    public:
        BV(){};
        ~BV(){}

        bool intersect(Ray ray)
        {
            float t_near = -INFINITY;
            float t_far = INFINITY;
            for (int i = 0; i < slab_normals.size(); i++)
            {
                Vec3f n = slab_normals[i];
                float n_dot_origin = n.dotProduct(ray.origin);
                float n_dot_direction = n.dotProduct(ray.direction);
                if (n_dot_direction != 0)
                {
                    if (n_dot_direction < 0) {n_dot_direction *= -1;}

                    float t_0 = (extents[i].x - n_dot_origin) / n_dot_direction;
                    float t_1 = (extents[i].y - n_dot_origin) / n_dot_direction;
                    if (t_0 > t_1) {return false;}
                    t_near = std::max(t_near, t_0);
                    t_far = std::min(t_far, t_1);
                }
            }
            if (t_near > t_far)
            {
                return false;
            }
            return true;
        }
        void constructCentroid() {
            centroid = (Vec3f(extents[0].x, extents[1].x, extents[2].x) + Vec3f(extents[0].y, extents[1].y, extents[2].y)) * 0.5;
        }
        std::vector<Vec3f> slab_normals = {Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1)};
        std::vector<Vec2f> extents;
        Vec3f centroid;
    };

} // namespace rt

#endif