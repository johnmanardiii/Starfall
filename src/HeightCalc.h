#pragma once
#include <math.h>

// hardcoded value from sin function that generates the terrain.
inline float heightCalc(float x, float z)
{
	return z * 0.2f + sin(x / 20.0) * sin(z / 20.0) * 20.0 * cos(x / 20.) * cos(z / 15.);
	// return z * .5;
}