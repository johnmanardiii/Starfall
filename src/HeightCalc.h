#include <math.h>

// hardcoded value from sin function that generates the terrain.
float heightCalc(float x, float z)
{
  return sin(x / 10.0) * sin(z / 10.0) * 10.0 * cos(x * 0.2) * cos(z * 0.1);
}