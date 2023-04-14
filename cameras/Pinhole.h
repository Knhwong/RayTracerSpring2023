
#ifndef PINHOLE_H_
#define PINHOLE_H_

#include "core/Camera.h"

namespace rt {

class Pinhole : public Camera {
public:
	Pinhole();
	Pinhole(int width, int height, int fov,  Vec3f origin, Vec3f lookAt, Vec3f up);

	~Pinhole(){};
	Ray calculateRay(int i, int j);
	void printCamera();
private:
	Vec3f origin;
	Vec3f lookAt;
	Vec3f up;
};

} // namespace rt

#endif
