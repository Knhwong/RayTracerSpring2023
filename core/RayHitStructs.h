/*
 * RayHitStructs.h
 */

#ifndef CORE_RAYHITSTRUCTS_H_
#define CORE_RAYHITSTRUCTS_H_

#include "math/geometry.h"

namespace rt {

/*
 * Ray structure definition
 */
enum RayType { PRIMARY, REFLECTION, REFRACTION, SHADOW};

struct Ray {

  RayType raytype;
  Vec3f direction;
  Vec3f origin;
  Ray(){};
  Ray(RayType raytype, Vec3f direction, Vec3f origin) : raytype(raytype), direction(direction), origin(origin) {}
};

struct Hit {
  bool isHit;
  Vec3f point;
  Vec3f normal;
  float distance;
  Hit(){};
  Hit(bool isHit):isHit(isHit){};
  Hit(bool isHit, Vec3f point, Vec3f normal): isHit(isHit), point(point), normal(normal){};
  Hit(bool isHit, Vec3f point, Vec3f normal, float dist): isHit(isHit), point(point), normal(normal), distance(dist){};
};

} // namespace rt

#endif /* CORE_RAYHITSTRUCTS_H_ */
