/*
 * LightSource.cpp
 *
 */
#include "LightSource.h"
#include "lights/PointLight.h"
#include <string>

namespace rt{

    LightSource* LightSource::createLight(Value& lightSpec) {
        if (!lightSpec.HasMember("type")){
            std::cerr<<"Light type not specified"<<std::endl;
            exit(-1);
	    }

        std::string type = lightSpec["type"].GetString();
        if (type == "pointlight"){
            printf("Point Light Detected\n");
            return new PointLight(
                parseVec3f(lightSpec["position"].GetArray()),
                parseVec3f(lightSpec["is"].GetArray()),
                parseVec3f(lightSpec["id"].GetArray()),
                lightSpec.HasMember("intensity") ? lightSpec["intensity"].GetFloat() : 1.0
            );
        }

        return 0;
    }
} //namespace rt

