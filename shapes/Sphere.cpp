#include "Sphere.h"
#include <math.h>
#define BIAS 1e-4
// For interior intersection!

namespace rt
{

	/**
	 * Computes whether a ray hit the specific instance of a sphere shape and
	 * returns the hit data
	 *
	 * @param ray cast ray to check for intersection with shape
	 *
	 * @return hit struct containing intersection information
	 *
	 */
	Hit Sphere::intersect(Ray ray)
	{

		Vec3f L = center - ray.origin;
		float t_ca = L.dotProduct(ray.direction);
		if (t_ca < 0)
		{
			return Hit(false);
		}
		float d = sqrt(L.dotProduct(L) - t_ca * t_ca);
		if (d > radius)
		{
			return Hit(false);
		}
		float t_hc = sqrt(radius * radius - d * d);

		float t;
		float t_0 = t_ca - t_hc;
		float t_1 = t_ca + t_hc;

		// We need this so that rays can be cast into the interior without immediately
		// intersecting. std::cout << "Intersect Test" << ray.origin <<
		// abs((ray.origin
		// - center).length()) << "\n";
		if (abs((ray.origin - center).length()) <= radius + BIAS)
		{
			t = t_1;
		}
		else
		{
			t = t_0;
		}

		Vec3f P = ray.origin + (t * ray.direction);
		Vec3f normal = (P - center).normalize();
		float distance = (P - ray.origin).length();

		return Hit(true, P, normal, distance);
	}

	Vec3f Sphere::shade(LightSource *light_source, Vec3f hit, Vec3f view,
						Vec3f normal)
	{
		// float u = (atan2(-(hit.z - center.z), hit.x - center.x) + M_PI) / (2*M_PI);
		// float v = (acos(-(hit.y - center.y)/radius))/M_PI;
		float u = normal.y / 2 + 0.5;
		float v = normal.x / 2 + 0.5;
		Vec3f color = material->getColor(u, v);
		return material->shade(light_source, hit, view, normal, color);
	}

	Sphere::Sphere(Vec3f c, float radius, std::string id, Material *mat)
		: Shape(id, mat), center(c), radius(radius)
	{
		for (Vec3f n : boundingVolume.slab_normals)
		{
			boundingVolume.extents.push_back(getExtents(n));
		}
	};

	Sphere *Sphere::createSphere(Value &d, Material *mat)
	{
		std::string id = (d.HasMember("id") ? d["id"].GetString() : "");
		Vec3f check = parseVec3f(d["center"].GetArray());
		std::cout << parseVec3f(d["center"].GetArray()) << " " << check << "\n";
		return new Sphere(check, d["radius"].GetFloat(), id, mat);
	}

	Vec2f Sphere::getExtents(Vec3f n)
	{
		Vec3f v = n * radius + center;
		float near = INFINITY;
		float far = -INFINITY;
		for (auto v :
			 std::vector<Vec3f>{(n * radius + center), (center - n * radius)})
		{
			float d = n.dotProduct(v);
			near = std::min(near, d);
			far = std::max(far, d);
		}
		return Vec2f(near, far);
		return Vec2f(0, 0);
	}

} // namespace rt
