#ifndef HEIGHTCALC_H
#define HEIGHTCALC_H
#include <math.h>
#include <glm/glm.hpp>
using namespace glm;
class HeightCalc {
public:
	static float heightCalc(float x, float z);
	static vec3 groundNormal(float x, float z);
};
#endif