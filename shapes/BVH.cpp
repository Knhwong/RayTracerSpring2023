/*
 * BVH.cpp
 *
 *
 */
#include "BVH.h"
#include "core/Shape.h"

namespace rt
{

    BVH::BVH(std::vector<Shape*> objects)
    {

        int longest_axis = 0;
        float axis_check = -INFINITY;
        for (int i = 0; i < boundingVolume.slab_normals.size(); i++)
        {

            float near = INFINITY;
            float far = -INFINITY;
            for (auto o : objects)
            {
                near = std::min(near, o->boundingVolume.extents[i].x);
                far = std::max(far, o->boundingVolume.extents[i].y);
            }
            boundingVolume.extents.push_back(Vec2f(near, far));
            boundingVolume.constructCentroid();
            // Find Longest Axis
            if (far - near > axis_check)
            {
                axis_check = far - near;
                longest_axis = i;
            }
        }
        if (objects.size() <= 2)
        {
            shapes = objects;
            leaf = true;
            return;
        }

        if (longest_axis == 0.0)
        {
            std::sort(objects.begin(), objects.end(), compareIntervalX);
        }
        else if (longest_axis == 1.0)
        {
            std::sort(objects.begin(), objects.end(), compareIntervalY);
        }
        else
        {
            std::sort(objects.begin(), objects.end(), compareIntervalZ);
        }
        int median = objects.size() / 2;
        std::vector<Shape*> left(objects.begin(),
                             objects.begin() + median);
        std::vector<Shape*> right(objects.begin() + median,
                              objects.end());
        shapes = objects;
        leftChild = new BVH(left);
        rightChild = new BVH(right);
    }

    bool BVH::intersect(Ray ray, std::vector<Shape*>& objects){
        if (!boundingVolume.intersect(ray))
        {
            return false;
        }
        if (leaf) {
            objects.insert( objects.end(), shapes.begin(), shapes.end());
            return true;
        } else{
            return leftChild->intersect(ray, objects) || rightChild->intersect(ray, objects);
        }
    }

} // namespace rt
