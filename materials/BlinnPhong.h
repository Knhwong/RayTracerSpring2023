/*
 * BlinnPhong.h
 *
 *
 */

#ifndef BLINNPHONG_H_
#define BLINNPHONG_H_

#include "core/Material.h"

namespace rt {

class BlinnPhong : public Material {
public:
  BlinnPhong(float ks, float kd, float kr, float specular, Vec3f diffuse)
      : Material(ks, kd, kr, specular, diffuse){};
  BlinnPhong(float ks, float kd, float kr, float specular, Vec3f diffuse, std::string t, float w, float h)
      : Material(ks, kd, kr, specular, diffuse, t, w, h){};
  virtual ~BlinnPhong(){};
  Vec3f shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal, Vec3f diffuseColor_);
};

} // namespace rt

#endif /* BLINNPHONG_H_ */
