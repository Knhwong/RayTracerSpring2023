/*
 * Scene.h
 *
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "rapidjson/document.h"

#include "core/LightSource.h"
#include "core/Shape.h"
#include "shapes/Sphere.h"
#include "shapes/Triangle.h"
#include "shapes/Plane.h"
#include "shapes/TriMesh.h"
#include "geometry.h"
#include "utils.h"
#include "Material.h"

using namespace rapidjson;

namespace rt{

class Scene {
public:

	Scene(){};

	void createScene(Value& scenespecs);
	
	std::vector<LightSource*> lightSources;
	std::vector<Shape*> shapes;

	Vec3f get_bg(){return backgroundColor;}

private:
	std::vector<Material*> materials;
	Vec3f backgroundColor;
};

} //namespace rt



#endif /* SCENE_H_ */
