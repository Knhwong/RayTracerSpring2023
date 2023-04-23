/*
 * RayTracer.cpp
 *
 */
#include "RayTracer.h"
#include "core/LightSource.h"
#include <bits/stdc++.h>
#include "utils.h"

#define SHADOW_BIAS 1e-2

namespace rt
{

	static int RayCasts = 0;
	static int RayHits = 0;
	static int RayIntersections = 0;

	/**
	 * Performs ray tracing to render a photorealistic scene
	 *
	 * @param camera the camera viewing the scene
	 * @param scene the scene to render, including objects and lightsources
	 * @param nbounces the number of bounces to consider for raytracing
	 *
	 * @return a pixel buffer containing pixel values in linear RGB format
	 */
	Vec3f *RayTracer::render(Camera *camera, Scene *scene, int nbounces)
	{
		int width = camera->getWidth();
		int height = camera->getHeight();
		Vec3f *pixelbuffer = new Vec3f[width * height];
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Shape *nearest_object;
				Ray ray = camera->calculateRay(j, i);
				pixelbuffer[j + i * width] = RayTrace(scene, ray, nbounces);
			}
		}
		printf("Rendering Complete, Intersections Tested: %d, Hits: %d \n", RayIntersections, RayHits);
		return pixelbuffer;
	}

	Hit RayTracer::NearestIntersection(Scene *scene, Ray ray, Shape *&object)
	{
		RayIntersections++;
		Hit nearest = Hit(false, Vec3f(0), Vec3f(0), INFINITY);
		for (auto shape : scene->shapes)
		{
			Hit p = shape->intersect(ray);
			if (p.isHit && p.distance < nearest.distance)
			{
				RayHits++;
				nearest = p;
				object = shape;
			}
		}
		return nearest;
	}

	float fresnel(float n1, float n2, float cosin1, float cosin2)
	{
		float F_parallel = pow((n2 * cosin1 - n1 * cosin2) / (n2 * cosin1 + n1 * cosin2), 2);
		float F_perpendicular = pow((n1 * cosin2 - n2 * cosin1) / (n1 * cosin2 + n2 * cosin1), 2);

		return 0.5 * (F_parallel + F_perpendicular);
	}

	Vec3f RayTracer::RayTrace(Scene *scene, Ray ray, int nbounces)
	{
		if (nbounces == 0)
			return Vec3f(0);
		Shape *obj;
		Hit hit = NearestIntersection(scene, ray, obj);
		if (hit.isHit == false)
			return scene->get_bg();

		// Shadow Testing
		Vec3f shading = Vec3f(0);
		for (auto *source : scene->lightSources)
		{

			Vec3f S = (source->get_position() - hit.point).normalize();
			Shape *dummy;
			Hit obstructed = NearestIntersection(scene, Ray(RayType::SHADOW, S, hit.point + hit.normal * SHADOW_BIAS), dummy);
			if (obstructed.isHit == true)
			{
				if (obstructed.distance > (source->get_position() - hit.point).length() || dummy->getRI() != 1)
				{
					obstructed.isHit = false;
				}
			}

			if (obstructed.isHit == false)
			{
				shading = shading + obj->shade(source, hit.point, -ray.direction, hit.normal);
			}
		}
		// Reflection

		Vec3f R = ray.direction - (2 * ray.direction.dotProduct(hit.normal) * hit.normal);
		Vec3f reflection = RayTrace(scene, Ray(RayType::REFLECTION, R, hit.point + hit.normal * SHADOW_BIAS), nbounces - 1);

		// Refraction
		Vec3f refraction = Vec3f(0);
		if (obj->getRI() != 1)
		{

			float n1 = 1;
			float n2 = obj->getRI();

			Vec3f transmission_normal = hit.normal;
			bool inside = (ray.direction.dotProduct(hit.normal) > 0);
			float c1 = -transmission_normal.dotProduct(ray.direction);
			if (c1 < 0)
			{
				c1 = -c1;
			}
			float n = n1 / n2;
			float c2 = 1 - n * n * (1 - c1 * c1);
			if (c2 > 0)
			{
				Vec3f T = (n * ray.direction + (n * c1 - sqrt(c2)) * transmission_normal).normalize();
				Vec3f p = hit.point - transmission_normal * SHADOW_BIAS;
				refraction = RayTrace(scene, Ray(RayType::REFRACTION, T, p), nbounces - 1);

				float F_t = fresnel(n1, n2, c1, c2);
				return F_t * (reflection*obj->getKr()) + ((1 - F_t) * refraction) * obj->getColor();
			}
		}

		return shading + (reflection * obj->getKr()) + (refraction * (1 - obj->getKr()));
	}

	/**
	 * Tonemaps the rendered image (conversion of linear RGB values [0-1] to low dynamic range [0-255]
	 *
	 * @param pixelbuffer the array of size width*height containing linear RGB pixel values
	 *
	 * @return the tonemapped image
	 */
	Vec3f *RayTracer::tonemap(Vec3f *pixelbuffer, int size)
	{

		for (int i = 0; i < size; ++i)
		{
			// std::max plays funny here when comparing to literals.
			const float m = 255;
			const float f = 0;
			pixelbuffer[i] = pixelbuffer[i] * m;
			pixelbuffer[i].x = std::min(pixelbuffer[i].x, m);
			pixelbuffer[i].y = std::min(pixelbuffer[i].y, m);
			pixelbuffer[i].z = std::min(pixelbuffer[i].z, m);

			pixelbuffer[i].x = std::max(pixelbuffer[i].x, f);
			pixelbuffer[i].y = std::max(pixelbuffer[i].y, f);
			pixelbuffer[i].z = std::max(pixelbuffer[i].z, f);
		}

		return pixelbuffer;
	}

} // namespace rt
