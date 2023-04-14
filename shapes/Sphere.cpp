#include "Sphere.h"
#include <math.h> 

namespace rt{


	/**
	 * Computes whether a ray hit the specific instance of a sphere shape and returns the hit data
	 *
	 * @param ray cast ray to check for intersection with shape
	 *
	 * @return hit struct containing intersection information
	 *
	 */
	Hit Sphere::intersect(Ray ray){
		
		Vec3f L = center - ray.origin;
		float t_ca = L.dotProduct(ray.direction);
		if (t_ca < 0) {
			return Hit(false);
		}
		float d = sqrt(L.dotProduct(L) - t_ca*t_ca);
		if (d > radius) {
			return Hit(false);
		}
		float t_hc = sqrt(radius*radius - d*d);

		float t;
		float t_0 = t_ca - t_hc;
		float t_1 = t_ca + t_hc;
		t = t_0;
		Vec3f P = ray.origin + (t*ray.direction);
		Vec3f normal = (P - center).normalize();

		return Hit(true, P, normal);
	}
	
	Vec3f Sphere::shade(LightSource* light_source, Vec3f hit, Vec3f view, Vec3f normal) {
		//float u = (atan2(-(hit.z - center.z), hit.x - center.x) + M_PI) / (2*M_PI);
		//float v = (acos(-(hit.y - center.y)/radius))/M_PI;
		float u = normal.y/2 + 0.5;
		float v = normal.x/2 + 0.5;
		Vec3f color = material->getColor(u, v);
		return material->shade(light_source, hit, view, normal, color);
	}

	Sphere::Sphere(Vec3f c, float radius, std::string id, Material* mat):Shape(id, mat), center(c), radius(radius){
		std::cout << center << c << "\n";
	};

	Sphere* Sphere::createSphere(Value& d, Material* mat) {
		std::string id = (d.HasMember("id") ? d["id"].GetString() : "");
		Vec3f check = parseVec3f(d["center"].GetArray());
		std:: cout << parseVec3f(d["center"].GetArray()) << " " << check << "\n";
		return new Sphere(check, d["radius"].GetFloat(), id, mat);
	}




} //namespace rt


