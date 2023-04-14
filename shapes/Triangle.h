/*
 * Triangle.h
 *
 *
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "core/Shape.h"
#include "rapidjson/document.h"

using namespace rapidjson;

namespace rt {

class Triangle : public Shape {
public:
  Triangle();
  Triangle(Vec3<float> v0, Vec3<float> v1, Vec3<float> v2, std::string id,
           Material *mat)
      : v0(v0), v1(v1), v2(v2), Shape(id, mat) {
    normal = (v1 - v0).crossProduct(v2 - v0).normalize();
    D = -normal.dotProduct(v0);
  };
  virtual ~Triangle(){};
  static Triangle *createTriangle(Value &d, Material *mat);
  Hit intersect(Ray ray);

  Vec3f shade(LightSource *light_source, Vec3f hit, Vec3f view, Vec3f normal);

private:
  Vec3<float> v0;
  Vec3<float> v1;
  Vec3<float> v2;
  Vec3<float> normal;
  float D;
};

} // namespace rt

#endif /* TRIANGLE_H_ */
