#include "Triangle.h"

namespace rt {

	Triangle* Triangle::createTriangle(Value& d, Material* mat) {
		std::string id = (d.HasMember("id") ? d["id"].GetString() : "");
		return new Triangle(
			parseVec3f(d["v0"].GetArray()),
			parseVec3f(d["v1"].GetArray()),
			parseVec3f(d["v2"].GetArray()),
			id, mat
		);

	}

	Vec3f Triangle::shade(LightSource *light_source, Vec3f hit, Vec3f view, Vec3f normal) {
		Vec3f color = material->getColor(0, 0);
		return material->shade(light_source, hit, view, normal, color);
	}

	Hit Triangle::intersect(Ray ray){
		if (normal.dotProduct(ray.direction) == 0) {
			return Hit(false);
		}
		float t = -(normal.dotProduct(ray.origin)+D) / normal.dotProduct(ray.direction);
		if (t < 0) {
			return Hit(false);
		}
		Vec3f P = ray.origin + t*ray.direction;
		float a = normal.dotProduct((P-v0).crossProduct(v1-v0));
		float b = normal.dotProduct((P-v1).crossProduct(v2-v1));
		float c = normal.dotProduct((P-v2).crossProduct(v0-v2));
		if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0)) {
			return Hit(true, P, normal);
		}

		return Hit(false);
	

	}
}

