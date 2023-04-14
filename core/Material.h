/*
 * Material.h
 *
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "math/geometry.h"
#include "core/LightSource.h"
#include <string>
#include "rapidjson/document.h"
#include "utils.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace rapidjson;
namespace rt{

class Material{
public:
    Material(float ks, float kd, float kr, float specular, Vec3f diffuse)
    :ks(ks), kd(kd), kr(kr), specularExponent(specular), diffuseColor(diffuse){};
    Material(float ks, float kd, float kr, float specular, Vec3f diffuse, std::string t, float w, float h);
    virtual ~Material(){};

    static Material* createMaterial(Value& mat);
    virtual Vec3f shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal, Vec3f diffuseColor_) = 0;
    void print();
    float getKr() {return kr;};
    Vec3f getColor(float u, float v);

protected:
    float ks = 0;
    float kd = 0;
    float kr = 0;
    float specularExponent = 0;
    Vec3f diffuseColor;
    std::string tPath = "";
    float tWidth;
    float tHeight;
    cv::Mat tex;
};


} //namespace rt



#endif /* MATERIAL_H_ */
