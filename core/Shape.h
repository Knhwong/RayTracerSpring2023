/*
 * Shape.h
 *
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "core/LightSource.h"
#include "core/Material.h"
#include "core/RayHitStructs.h"
#include "math/geometry.h"
#include "rapidjson/document.h"
#include "shapes/BV.h"
#include "string"
#include "utils.h"

namespace rt {

class Shape {
public:
  Shape(){};
  Shape(std::string id, Material *mat) : id(id), material(mat){};

  virtual ~Shape(){};

  virtual Hit intersect(Ray) = 0;

  virtual Vec3f shade(LightSource *light_source, Vec3f hit, Vec3f view,
                      Vec3f normal) = 0;


  float getKr() { return material->getKr(); }
  float getRI() { return material->getRI(); }
  Vec3f getColor() { return material->getColor(0.0, 0.0); }
  BV boundingVolume;

protected:
  Material *material;
  std::string id;
};

} // namespace rt

#endif