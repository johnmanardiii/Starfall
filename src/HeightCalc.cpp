#pragma once
#include "HeightCalc.h"

// hardcoded value from sin function that generates the terrain.
float HeightCalc::heightCalc(float x, float z)
{
	return z * 0.2f + sin(x / 20.0) * sin(z / 20.0) * 20.0 * cos(x / 20.) * cos(z / 15.);
}

vec3 HeightCalc::groundNormal(float x, float z)
{
	vec3 a = vec3(x, heightCalc(x, z), z);
	float xoff = x + 0.01f,
		zoff = z + 0.01f;
	vec3 b = vec3(xoff, heightCalc(xoff, zoff), zoff);
	zoff = z - 0.01f;
	vec3 c = vec3(xoff, heightCalc(xoff, zoff), zoff);
	vec3 ab = a - b;
	ab = normalize(ab);
	vec3 ac = a - c;
	ac = normalize(ac);
	return normalize(cross(ab, ac));
}