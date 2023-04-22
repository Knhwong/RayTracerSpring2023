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
		Vec3f forward = (pos - lookat).normalize();
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

     inline Vec3f calculateBarycentric(Vec3f V0, Vec3f V1, Vec3f V2, Vec3f point)
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

}

#endif