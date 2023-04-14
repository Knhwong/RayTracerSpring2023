#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "geometry.h"
#include <cmath>

#ifndef UTILS_H_
#define UTILS_H_

using namespace rapidjson;

namespace rt {

    inline Vec3f parseVec3f(Value doc) {
	Vec3f out;
	for (SizeType i = 0; i < doc.Size(); i++) {
		out[i] = doc[i].GetFloat();
	}
	float y = out.y;
	return out;
	}

	inline float degToRad(float degrees) {
		return degrees * (M_PI / 180);
	}
	// With help from
	// https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function/framing-lookat-function.html
	/*inline Matrix44f buildViewMatrix(Vec3f& origin, Vec3f& lookat, Vec3f& up) 
	{ 
		Vec3f forward = origin - lookat; 
		forward = forward.normalize(); 
		Vec3f right = up.crossProduct(forward).normalize(); 
		Vec3f newup = forward.crossProduct(right); 
		
		Matrix44f m(
		right.x,   right.y,   right.z, 0,
		newup.x,   newup.y,   newup.z, 0,
		forward.x, forward.y, forward.z, 0,
		origin.x,    origin.y,    origin.z, 1
		);
		return m;
  };*/
  	inline Matrix44f buildViewMatrix(Vec3f& pos, Vec3f& lookat, Vec3f& up) 
	{ 
		Vec3f forward = (pos- lookat).normalize();
		Vec3f right = up.normalize().crossProduct(forward).normalize();
		Vec3f newup = forward.crossProduct(right); 
		
		Matrix44f m(
		right.x,   right.y,   right.z, 0,
		newup.x,   newup.y,   newup.z, 0,
		forward.x, forward.y, forward.z, 0,
		pos.x,    pos.y,    pos.z, 1
		);
		return m;
  };

}

#endif