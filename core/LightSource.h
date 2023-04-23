/*
 * LightSource.h
 *
 */

#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "math/geometry.h"
#include "core/utils.h"

namespace rt{

class LightSource {
public:
    LightSource(Vec3f p, Vec3f is_, Vec3f id_, float intensity): position(p), is(is_), id(id_){
        is = is.normalize() * intensity;
        id = id.normalize() * intensity;
        printf("Is: %f, Id: %f\n", is.x, id.x);
    }
    static LightSource* createLight(Value& lightSpecs);

    Vec3f get_position() {return position;}
    Vec3f get_is() {return is;}
    Vec3f get_id() {return id;}
    void print() {std::cout << position << " " << is << " " << id << "\n";}

private:
    Vec3f position;
    Vec3f is;
    Vec3f id;
};

} //namespace rt



#endif /* LIGHTSOURCE_H_ */
