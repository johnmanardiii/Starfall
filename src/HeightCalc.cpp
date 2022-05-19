#include "HeightCalc.h"
#include "stb_image.h"
#include <string>
#include <iostream>

float HeightCalc::GetHeight(float x, float z)
{
	// ((pos / 100.0f) * mapW) / 3.0f
	int ix = (x / 300.0f) * mapW,
		iz = (z / 300.0f) * mapH;
	//return z * 0.2f + sin(x / 20.0) * sin(z / 20.0) * 20.0 * cos(x / 20.) * cos(z / 15.);
	if (ix < 0 || ix >= mapW || iz < 0 || iz >= mapH)
	{
		return 0;
	}
	return *(heightMap + ix + iz * mapW) * 40.0f;
}

void HeightCalc::Init()
{
	const std::string resourceDir = "../resources";
	int nrChannels;
	unsigned char* data =
		stbi_load((resourceDir + "/HeightMaps/Mountain.jpg").c_str(), &mapW, &mapH, &nrChannels, 1);
	heightMap = (float*) malloc(sizeof(float) * mapW * mapH);
	if (heightMap == nullptr)
	{
		std::cerr << "Memory overflow with mountain heighmap" << std::endl;
		exit(1);
	}
	for (int i = 0; i < mapW; i++)
	{
		for (int j = 0; j < mapH; j++)
		{
			unsigned char h = data[i + j * mapW];
			heightMap[i + j * mapW] = h / 255.0f;
		}
	}
	stbi_image_free(data);
}