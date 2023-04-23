/*
 * BVH.h
 *
 *
 */

#ifndef BVH_H_
#define BVH_H_

#include "core/Shape.h"

// https://www.haroldserrano.com/blog/visualizing-the-boundary-volume-hierarchy-collision-algorithm
namespace rt
{
    static bool compareIntervalX(Shape* a, Shape* b)
    {
        return (a->boundingVolume.centroid.x > b->boundingVolume.centroid.x);
    }
    static bool compareIntervalY(Shape* a, Shape* b)
    {
        return (a->boundingVolume.centroid.y > b->boundingVolume.centroid.y);
    }
    static bool compareIntervalZ(Shape* a, Shape* b)
    {
        return (a->boundingVolume.centroid.z > b->boundingVolume.centroid.z);
    }
    class BVH
    {
    public:
        BVH(){};
        BVH(std::vector<Shape*> shapes);
        bool intersect(Ray ray, std::vector<Shape*>& objects);

    private:
        BVH *leftChild;
        BVH *rightChild;
        BV boundingVolume;
        std::vector<Shape*> shapes;
        bool leaf = false;
    };

} // namespace rt

#endif /* BVH_H_ */
