#ifndef PLANE_H_
#define PLANE_H_

#include "core/Shape.h"
#include "rapidjson/document.h"

using namespace rapidjson;
namespace rt {

class Plane : public Shape {
  // Actually are Quads
public:
  Plane();
  Plane(Vec3<float> v0, Vec3<float> v1, Vec3<float> v2, Vec3<float> v3,
        std::string id, Material *mat)
      : v0(v0), v1(v1), v2(v2), v3(v3), Shape(id, mat) {
    for (Vec3f n : boundingVolume.slab_normals) {
      boundingVolume.extents.push_back(getExtents(n));
    }
  };
  virtual ~Plane(){};
  Vec3f shade(LightSource *light_source, Vec3f hit, Vec3f view, Vec3f normal);

  static Plane *createPlane(Value &d, Material *mat);
  Hit intersect(Ray ray);
  Vec2f getExtents(Vec3f n);

private:
  Vec3<float> v0;
  Vec3<float> v1;
  Vec3<float> v2;
  Vec3<float> v3;
  Hit intersect_triangle(Ray ray, Vec3f V0, Vec3f V1, Vec3f V2);
  //hack to know which triangle was hit!
  int index = 0;
};

} // namespace rt

#endif