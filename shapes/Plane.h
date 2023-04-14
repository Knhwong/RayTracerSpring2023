#ifndef PLANE_H_
#define PLANE_H_

#include "core/Shape.h"
#include "rapidjson/document.h"

using namespace rapidjson;
namespace rt{

class Plane: public Shape{
    //Actually are Quads
    public:
        Plane();
        Plane(Vec3<float> v0, Vec3<float> v1, Vec3<float> v2, Vec3<float> v3, std::string id, Material* mat)
        : v0(v0), v1(v1), v2(v2), v3(v3), Shape(id, mat) {};
        virtual ~Plane(){};
        Vec3f shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal);

        static Plane* createPlane(Value& d, Material* mat);
        Hit intersect(Ray ray);
    private:
        Vec3<float> v0;
        Vec3<float> v1;
        Vec3<float> v2;
        Vec3<float> v3;
        Hit intersect_triangle(Ray ray, Vec3f V0, Vec3f V1, Vec3f V2);
};



} 

#endif