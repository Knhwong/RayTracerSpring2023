#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "core/LightSource.h"

namespace rt {

class PointLight : public LightSource {
    public:
    PointLight(Vec3f p, Vec3f is, Vec3f id, float intensity);
};

} // namespace rt
#endif
