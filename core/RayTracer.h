/*
 * RayTracer.h
 *
 */
#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "math/geometry.h"
#include "core/Camera.h"
#include "core/Scene.h"

namespace rt{



/*
 * Raytracer class declaration
 */
class RayTracer {
public:

	RayTracer();

	static Vec3f* render(Camera* camera, Scene* scene, int nbounces);
	static Vec3f* tonemap(Vec3f* pixelbuffer, int size);
private:
	static Hit NearestIntersection(Scene* scene, Ray ray, Shape*& object);
	static Vec3f RayTrace(Scene* scene, Ray ray, int nbounces);
	static Vec3f Refract(Scene scene, Ray ray, Hit hit, Shape* obj);
};




} //namespace rt



#endif /* RAYTRACER_H_ */

