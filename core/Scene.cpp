#include "Scene.h"

namespace rt {
/**
 * Parses json scene object to generate scene to render
 *
 * @param scenespecs the json scene specificatioon
 */
void Scene::createScene(Value &scenespecs) {
  backgroundColor = parseVec3f(scenespecs["backgroundcolor"].GetArray());
  std::cout << "Background Color is: " << backgroundColor << "\n";
  // Build Lights
  for (Value &v : scenespecs["lightsources"].GetArray()){
    lightSources.push_back(LightSource::createLight(v));
    lightSources[0]->print();
  } 


  // Build Shapes
  int i = 0;
  for (Value &v : scenespecs["shapes"].GetArray()) {
    std::string type = v["type"].GetString();
    if (type == "sphere") {
      printf("Adding Sphere\n");
      Material* mat =  Material::createMaterial(v["material"]);
      shapes.push_back(Sphere::createSphere(v, mat));
    }
    if (type == "triangle") {
      printf("Adding Triangle\n");
      Material* mat =  Material::createMaterial(v["material"]);
      shapes.push_back(Triangle::createTriangle(v, mat));
    }
    if (type == "plane") {
      printf("Adding Plane\n");
      Material* mat =  Material::createMaterial(v["material"]);
      shapes.push_back(Plane::createPlane(v, mat));
    }
    if (type == "trimesh"){
        printf("Adding Trimesh\n");
        Material* mat =  Material::createMaterial(v["material"]);
        TriMesh::createTriMesh(v, mat);
    }
  }
}

} // namespace rt
