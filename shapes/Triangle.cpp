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

	Vec3f Triangle::calculateBarycentric(Vec3f V0, Vec3f V1, Vec3f V2, Vec3f point)
    {
        Vec3f v0 = V1 - V0, v1 = V2 - V0, v2 = point - V0;
        float d00 = v0.dotProduct(v0);
        float d01 = v0.dotProduct(v1);
        float d11 = v1.dotProduct(v1);
        float d20 = v2.dotProduct(v0);
        float d21 = v2.dotProduct(v1);
        float denom = d00 * d11 - d01 * d01;

        Vec3f uv;
        uv.y = (d11 * d20 - d01 * d21) / denom;
        uv.z = (d00 * d21 - d01 * d20) / denom;
        uv.x = 1.0 - uv.y - uv.z;
        return uv;
    }

	Vec2f Triangle::getExtents(Vec3f n) {
        float near = INFINITY;
        float far = -INFINITY;
        for (auto v : std::vector<Vec3f>{v0, v1, v2})
        {
            float d = n.dotProduct(v);
            near = std::min(near, d);
            far = std::max(far, d);
        }
        return Vec2f(near, far);
  	}

	Vec3f Triangle::shade(LightSource *light_source, Vec3f hit, Vec3f view, Vec3f normal) {
		Vec3f bary = calculateBarycentric(v0, v1, v2, hit);
		Vec3f color = material->getColor(bary.x, bary.y);
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
			float distance = (P - ray.origin).length();
			return Hit(true, P, normal, distance);
		}

		return Hit(false);
	

	}
}

