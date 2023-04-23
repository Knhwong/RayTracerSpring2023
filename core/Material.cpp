#include "Material.h"
#include "materials/BlinnPhong.h"
namespace rt {

Material::Material(float ks, float kd, float kr, int specular, float ri, Vec3f diffuse, std::string t, float w, float h)
    :ks(ks), kd(kd), kr(kr), specularExponent(specular), refractiveIndex(ri), diffuseColor(diffuse), tPath(t), tWidth(w), tHeight(h)
    {
    tex = cv::imread(tPath, cv::IMREAD_COLOR);
    std::cout << "Width" << tex.size().width << "\n";
    std::cout << "Height" << tex.size().height << "\n";
    }

Vec3f Material::getColor(float u, float v) {
  if (tPath == "") {return diffuseColor;}
  int x = u * tWidth;
  int y = v * tHeight;
  float b = tex.at<cv::Vec3b>(y, x)[0] / 255.0;
  float g = tex.at<cv::Vec3b>(y, x)[1] / 255.0;
  float r = tex.at<cv::Vec3b>(y, x)[2] / 255.0;
  return Vec3f(r,g,b);
}

Material* Material::createMaterial(Value& mat) {
  int s = mat["specularexponent"].GetFloat();
  if (mat.HasMember("tPath")) {
    std::string path = mat["tPath"].GetString();
    return new BlinnPhong(mat["ks"].GetFloat(), 
                        mat["kd"].GetFloat(),
                        (mat.HasMember("kr") ? mat["kr"].GetFloat() : 0.0),
                        mat["specularexponent"].GetInt(),
                        (mat.HasMember("refractiveIndex") ? mat["refractiveIndex"].GetFloat() : 1.0),
                        parseVec3f(mat["diffusecolor"].GetArray()), 
                        path,
                        mat["tWidth"].GetFloat(), 
                        mat["tHeight"].GetFloat());
  } else {
    return new BlinnPhong(mat["ks"].GetFloat(), 
                        mat["kd"].GetFloat(),
                        (mat.HasMember("kr") ? mat["kr"].GetFloat() : 0.0),
                        mat["specularexponent"].GetInt(),
                        (mat.HasMember("refractiveIndex") ? mat["refractiveIndex"].GetFloat() : 1.0),
                        parseVec3f(mat["diffusecolor"].GetArray())
                        );
  }
}

void Material::print() {
  printf("Material Parsed: %f, %f, %f\n", kd, ks, kr);
}

} // namespace rt
