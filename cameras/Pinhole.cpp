#include <iostream>
#include "Pinhole.h"

namespace rt {

Pinhole::Pinhole(int width, int height, int fov, Vec3f origin, Vec3f lookAt, Vec3f up)
    : Camera(width, height, fov), origin(origin), lookAt(lookAt), up(up) {
		viewMatrix = buildViewMatrix(origin, lookAt, up);
		std::cout << "Camera Built: \n" << origin << "\n" << lookAt << "\n" << up << "\n" << viewMatrix << "\n";
	}

// 800 * 800 Matrix - Hence 400,400 should be 0,0
// Using 0-1 for NDC space?
Ray Pinhole::calculateRay(int x, int y) {
	float ncd_x = (x + 0.5) / width;
	float ncd_y = (y + 0.5) / height;
	float pixel_x = ((2 * ncd_x) - 1) * (width/float(height)) * tan(degToRad(fov)*0.5);
	float pixel_y = (1 - (2 * ncd_y)) * tan(degToRad(fov)*0.5);

	Vec3f ray = Vec3f(pixel_x, pixel_y, -1);
	viewMatrix.multVecMatrix(Vec3f(pixel_x, pixel_y, -1), ray);
	Vec3f center = Vec3f(0);
	viewMatrix.multVecMatrix(Vec3f(0), center);
	
	return Ray(RayType::PRIMARY, (ray-center).normalize(), center);
 }

/**
 * Prints camera data
 * this function implements the abstract function of the base Camera class
 */
void Pinhole::printCamera() {
  printf("I am a pinhole camera! \n");
  printf("width: %dpx, height: %dpx, fov:%d \n", width, height, fov);
  std::cout << viewMatrix << "\n";
}

} // namespace rt
