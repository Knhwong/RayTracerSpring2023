/*
 * Sphere.h
 *
 *
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "math/geometry.h"
#include "core/RayHitStructs.h"
#include "core/Shape.h"
#include "rapidjson/document.h"

using namespace rapidjson;
namespace rt{

class Sphere:public Shape{

public:

	//
	// Constructors
	//
	Sphere();
	Sphere(Vec3f c, float radius, std::string id, Material* mat);
	virtual ~Sphere(){}
	Vec3f shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal);


	//
	// Functions that need to be implemented, since Sphere is a subclass of Shape
	//
	Hit intersect(Ray ray);
	static Sphere* createSphere(Value& d, Material* mat);
	Vec2f getExtents(Vec3f n);

private:
	Vec3f center;
	float radius;

};



} //namespace rt




#endif /* SPHERE_H_ */
