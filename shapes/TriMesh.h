/*
 * TriMesh.h
 *
 *
 */

#ifndef TRIMESH_H_
#define TRIMESH_H_

#include "core/Shape.h"
#include "core/obj_loader.h"

namespace rt{

class TriMesh : public Shape{

    public:
        TriMesh();
        TriMesh(Mesh mesh, Vec3f center, std::string id, Material* mat);
        virtual ~TriMesh(){};
        static TriMesh* createTriMesh(Value& d, Material* mat);
        Vec3f shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal);
        Hit intersect_triangle(Ray ray, Vec3f V0, Vec3f V1, Vec3f V2, Vec3f normal, int index);
        Hit intersect(Ray ray);
        //https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-acceleration-structure/bounding-volume.html
        std::vector<Vec3f> getBoundingBox();
        Vec3f calculateBarycentric(Vec3f V0, Vec3f V1, Vec3f V2, Vec3f point);

    private:
        std::vector<Vec3f> vertices;
        std::vector<Vec3<int>> vI;
        std::vector<Vec3f> normals;
        std::vector<Vec3f> faceNormals;
        std::vector<Vec3<int>> nI;
        std::vector<Vec2<float>> uv;
        std::vector<Vec3<int>> uvI;
        Vec3f center;
        //hack to get barycentric in shade;
        int index_ = 0;

};



} //namespace rt




#endif /* TRIMESH_H_ */
