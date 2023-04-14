/*
 * Camera.cpp
 *
 */
#include <iostream>

#include "Camera.h"
#include "cameras/Pinhole.h"
#include "cameras/ThinLens.h"

namespace rt{

	Camera::~Camera(){};



/**
 * Factory function that returns camera subclass based on camera specifications
 *
 * @param cameraSpecs camera specifications json object
 *
 * @return camera subclass instance
 *
 */
Camera* Camera::createCamera(Value& cameraSpecs){

	//check if cameratype is defined

	if (!cameraSpecs.HasMember("type")){
		std::cerr<<"Camera type not specified"<<std::endl;
		exit(-1);
	}

	std::string cameraType=cameraSpecs["type"].GetString();

	//return camera object based on camera specs
	if (cameraType.compare("pinhole")==0){
		return new Pinhole(cameraSpecs["height"].GetInt(),
				cameraSpecs["width"].GetInt(),
				cameraSpecs["fov"].GetInt(),
				parseVec3f(cameraSpecs["position"].GetArray()),
				parseVec3f(cameraSpecs["lookat"].GetArray()),
				parseVec3f(cameraSpecs["up"].GetArray())
				);

	}else if (cameraType.compare("thinlens")==0){
		return new ThinLens();
	}

	return 0;

}



} //namespace rt
