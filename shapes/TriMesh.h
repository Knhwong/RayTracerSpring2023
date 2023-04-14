/*
 * TriMesh.h
 *
 *
 */

#ifndef TRIMESH_H_
#define TRIMESH_H_

#include "core/Shape.h"


namespace rt{

class TriMesh: public Shape {

    public:
        TriMesh();
        TriMesh(std::vector<std::array<double, 3>> _vertices, std::vector<std::vector<size_t>> face_indexes, std::string id, Material* mat);
        virtual ~TriMesh();
        static TriMesh* createTriMesh(Value& d, Material* mat);
        Vec3f shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal);
        Hit intersect(Ray ray);
    private:
        std::vector<Vec3f> vertices;
        std::vector<Vec3<int>> indices;
        std::vector<Vec3f> normals;
        Vec3f center;

};



} //namespace rt




#endif /* TRIMESH_H_ */
