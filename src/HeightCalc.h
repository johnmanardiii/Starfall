#pragma once
#include <math.h>

class HeightCalc
{
private:
	float* heightMap;
	int mapW;
	int mapH;
public:
	float GetHeight(float x, float z);
	void Init();
	static HeightCalc& GetInstance() {
		static HeightCalc instance;
		return instance;
	}
};