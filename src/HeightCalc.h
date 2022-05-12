#pragma once
#include <math.h>

// hardcoded value from sin function that generates the terrain.
inline float heightCalc(float x, float z)
{
	//return z * 0.2f + sin(x / 20.0) * sin(z / 20.0) * 20.0 * cos(x / 20.) * cos(z / 15.);
	float dist = fabs(x - 20.0f * sin(z / 100.0f) * cos(z / 50.0f));
	dist = (std::max)(30.0f, dist);
	dist -= 30.0f;
	return z * 0.6f + (std::min)(50.0f, dist);
}