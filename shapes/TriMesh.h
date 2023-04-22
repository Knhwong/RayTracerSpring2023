/*
 * TriMesh.h
 *
 *
 */

#ifndef TRIMESH_H_
#define TRIMESH_H_

#include "core/Shape.h"
#include "core/obj_loader.h"
#include "shapes/BVH.h"

namespace rt{

class TriMesh : public Shape{

    public:
        TriMesh();
        TriMesh(Mesh mesh, Vec3f center, float scale, std::string id, Material* mat);
        virtual ~TriMesh(){};
        static TriMesh* createTriMesh(Value& d, Material* mat);
        Vec3f shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal);
        Vec3f shade_face(LightSource *light_source, Vec3f hit, Vec3f view, Vec3f normal, int i);
        Hit intersect_triangle(Ray ray, Vec3f V0, Vec3f V1, Vec3f V2, Vec3f normal, int index);
        Hit intersect(Ray ray);
        Hit intersect_face(int i, Ray ray);
        //https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-acceleration-structure/bounding-volume.html
        Vec2f getExtents(Vec3f n);
        Vec3f calculateBarycentric(Vec3f V0, Vec3f V1, Vec3f V2, Vec3f point);
    private:
        std::vector<Vec3f> vertices;
        std::vector<Vec3<int>> vI;
        std::vector<Vec3f> normals;
        std::vector<Vec3f> faceNormals;
        std::vector<Vec3<int>> nI;
        std::vector<Vec2<float>> uv;
        std::vector<Vec3<int>> uvI;
        Vec3f center = Vec3f(0, 0, 0);
        float scale = 1;
        //hack to get barycentric in shade;
        int index_ = 0;
        std::vector<Shape*> triangles;
        BVH bvh;

};

class TriMeshTriangle : public Shape{

    public:
        TriMeshTriangle(int index, TriMesh* parent, Vec3f v0, Vec3f v1, Vec3f v2): indexs(index), v0(v0), v1(v1), v2(v2), parent(parent), Shape("Trig", nullptr){
            for (Vec3f n : boundingVolume.slab_normals) {
                boundingVolume.extents.push_back(getExtents(n, v0, v1, v2));
                boundingVolume.constructCentroid();
            }

            centroid = (v0 + v1 + v2) * (1.0/3.0);
        };
        virtual ~TriMeshTriangle(){};
        Vec3f shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal);
        Hit intersect(Ray ray);
        Vec2f getExtents(Vec3f n, Vec3f v0, Vec3f v1, Vec3f v2) {
            float near = INFINITY;
            float far = -INFINITY;
            for (auto v : std::vector<Vec3f>{v0, v1, v2})
            {
                float d = n.dotProduct(v);
                near = std::min(near, d);
                far = std::max(far, d);
            }
            return Vec2f(near, far);
        }
    private:
        int indexs;
        TriMesh* parent;
        Vec3f v0;
        Vec3f v1;
        Vec3f v2;
        Vec3f centroid;
};



} //namespace rt




#endif /* TRIMESH_H_ */
