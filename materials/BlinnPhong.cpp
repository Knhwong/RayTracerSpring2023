/*
 * BlinnPhong.cpp
 *
 *
 */

#include "BlinnPhong.h"
//Equations sourced from sites; Previous knowledge from Rasterization Engine applied 
//https://learnopengl.com/Lighting/Basic-Lighting
//https://learnopengl.com/Advanced-Lighting/Advanced-Lighting


namespace rt{
    
    Vec3f BlinnPhong::shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal, Vec3f diffuseColor_){
        
        //diffuse
        Vec3f lightDir = light_source->get_position() - hit;
        float dist = 1/lightDir.length();
        lightDir = lightDir.normalize();

        Vec3f diffuse = normal.dotProduct(lightDir) * kd * diffuseColor_ * light_source->get_id();

        Vec3f half = (lightDir+view).normalize();
        float specular_intensity = pow(std::max(half.dotProduct(normal), float(0)), specularExponent);
        Vec3f specular = specular_intensity * ks * light_source->get_is();
            
        return (diffuse+specular) * dist;
    }
} //namespace rt
