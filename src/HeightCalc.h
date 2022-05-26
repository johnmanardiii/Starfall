#pragma once
#include <math.h>
#include <memory>
#include <vector>
using namespace std;

class HeightCalc
{
private:
	shared_ptr<vector<vector<float>>> heightMap;
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