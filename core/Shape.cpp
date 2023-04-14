/*
 * Shape.cpp
 *
 */
#include "Shape.h"

namespace rt{
    Vec3f Shape::shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal) {
        Vec3f color = material->getColor(0, 0);
        return material->shade(light_source, hit, view, normal, color);
    }
} //namespace rt

