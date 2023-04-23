#include "Plane.h"
#include "core/utils.h"

namespace rt {


	Vec2f Plane::getExtents(Vec3f n) {
        float near = INFINITY;
        float far = -INFINITY;
        for (auto v : std::vector<Vec3f>{v0, v1, v2, v3})
        {
            float d = n.dotProduct(v);
            near = std::min(near, d);
            far = std::max(far, d);
        }
        return Vec2f(near, far);
	}

	Plane* Plane::createPlane(Value& d, Material* mat){
		std::string id = (d.HasMember("id") ? d["id"].GetString() : "");
		return new Plane(
			parseVec3f(d["v0"].GetArray()),
			parseVec3f(d["v1"].GetArray()),
			parseVec3f(d["v2"].GetArray()),
			parseVec3f(d["v3"].GetArray()),
			id, mat
		);
	}

	Vec3f Plane::shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal) {
		Vec3f uv;
		if (index == 0) {
			uv = calculateBarycentric(v2, v0, v3, hit);
			uv.y = 1 - uv.y;
		} else {
			uv = calculateBarycentric(v0, v2, v1, hit);
			uv.x = 1 - uv.x;
		}

		//float y = (hit.y - v0.y)/abs(v0.y - v3.y);
		//float x = (hit.x + v0.x)/abs(v1.x - v0.x);
		//float z = (hit.z + v0.z)/abs(v1.z - v0.z);

		
		Vec3f color = material->getColor(uv.x, uv.y);
		return material->shade(light_source, hit, view, normal, color);
	}

	Hit Plane::intersect_triangle(Ray ray, Vec3f V0, Vec3f V1, Vec3f V2){
		Vec3f normal = (V1 - V0).crossProduct(V2 - V0).normalize();
		float D = -normal.dotProduct(v0);
		if (normal.dotProduct(ray.direction) == 0) {
			return Hit(false);
		}
		float t = -(normal.dotProduct(ray.origin)+D) / normal.dotProduct(ray.direction);
		if (t < 0) {
			return Hit(false);
		}
		Vec3f P = ray.origin + t*ray.direction;
		float a = normal.dotProduct((P-V0).crossProduct(V1-V0));
		float b = normal.dotProduct((P-V1).crossProduct(V2-V1));
		float c = normal.dotProduct((P-V2).crossProduct(V0-V2));
		if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0)) {
			float distance = (P - ray.origin).length();
			return Hit(true, P, normal, distance);
		}
		return Hit(false);
	}

	Hit Plane::intersect(Ray ray){
		Hit left = intersect_triangle(ray, v0, v3, v2);
		if (left.isHit) {
			index = 0;
			return left;
		}
		Hit right = intersect_triangle(ray, v0, v2, v1);
		if (right.isHit) {
			index = 1;
			return right;
		}

		return Hit(false);
	}

}