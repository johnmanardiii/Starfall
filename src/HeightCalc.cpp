#include "HeightCalc.h"
#include "stb_image.h"
#include <string>
#include <iostream>
#include <cassert>


// https://helloacm.com
inline float
BilinearInterpolation(float heights[], float x1, float x2, float y1, float y2, float x, float y)
{
	float q11 = heights[0],
		q12 = heights[1],
		q21 = heights[2],
		q22 = heights[3];
	float x2x1, y2y1, x2x, y2y, yy1, xx1;
	x2x1 = x2 - x1;
	y2y1 = y2 - y1;
	x2x = x2 - x;
	y2y = y2 - y;
	yy1 = y - y1;
	xx1 = x - x1;
	return 1.0 / (x2x1 * y2y1) * (
		q11 * x2x * y2y +
		q21 * xx1 * y2y +
		q12 * x2x * yy1 +
		q22 * xx1 * yy1
		);
}

float BilinearInterp(float heights[], float tx, float tz)
{
	float a = heights[0] * (1 - tx) + heights[1] * tx;
	float b = heights[2] * (1 - tx) + heights[2] * tx;
	return a * (1 - tz) + b * tz;
}
float HeightCalc::GetHeight(float x, float z)
{
	// why negative x
	// (((pos + 50.f) / 100.0f) * mapW) / 4.0f

	// lower x and z values to use for bilinear
	float imgX = (((-x + 50.0f) / 4000.0f) * mapW),
		imgZ = (((z + 50.0f) / 4000.0f) * mapH);
	int Lx = floor(imgX - 0.5f);
	int	Lz = floor(imgZ - 0.5f);
	float heights[4];
	for (int x = 0; x < 2; x++)
	{
		for (int z = 0; z < 2; z++)
		{
			// if outside of buffer, skip the pixel
			if (Lx + x < 0 || Lx + x >= mapW || Lz + z < 0 || Lz + z >= mapH)
			{
				heights[x + z * 2] = 0.0f;
			}
			else
			{
				heights[x + z * 2] = (*heightMap)[Lx + x][Lz + z] * 300;
			}
		}
	}
	return BilinearInterp(heights, imgX - Lx, imgZ - Lz) + 2.0f;
}

void HeightCalc::Init()
{
	const std::string resourceDir = "../resources";
	int nrChannels;
	// flipped vertically so the image data is bottom left to top right
	//stbi_set_flip_vertically_on_load(true);

	// load data as (grey, alpha) to get averaged color values
	unsigned char* data =
		stbi_load((resourceDir + "/HeightMaps/Mountain.jpg").c_str(), &mapW, 
			&mapH, &nrChannels, 2);
	heightMap = std::make_shared<vector<vector<float>>>();
	if (heightMap == nullptr)
	{
		std::cerr << "Memory overflow with mountain heightmap" << std::endl;
		exit(1);
	}
	// take char data, convert to 0..1 float value, and store in float buffer
	for (int i = 0; i < mapW; i++)
	{
		heightMap->emplace_back(vector<float>());
		for (int j = 0; j < mapH; j++)
		{
			unsigned char h = data[i * 2 + (j * 2) * mapW];
			(*heightMap)[i].push_back(h / 255.0f);
		}
	}
	stbi_image_free(data);
}