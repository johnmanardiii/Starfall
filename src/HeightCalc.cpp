#include "HeightCalc.h"
#include "stb_image.h"
#include <string>
#include <iostream>
#include <cassert>


// https://helloacm.com
inline float
BilinearInterpolation(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y)
{
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

float HeightCalc::GetHeight(float x, float z)
{

	// ((pos / 100.0f) * mapW) / 3.0f
	/*int ix = round(((x + 50.0f) / 300.0f) * mapW);
	int	iz = round(((z + 50.0f) / 300.0f) * mapH);*/

	int ix = round(((-x + 50.0f) / 400.0f) * mapW);
	int	iz = round(((z + 50.0f) / 400.0f) * mapH);

	float height = 0.0f;
	int numSamples = 0;
	// TODO: Bilinear filtering
	for (int x = -1; x < 2; x++)
	{
		for (int z = -1; z < 2; z++)
		{
			// if outside of buffer, skip the pixel
			if (ix + x < 0 || ix + x >= mapW || iz + z < 0 || iz + z >= mapH)
			{
				height += 0.0f;
			}
			else
			{
				height += (*heightMap)[ix + x][iz + z] * 100;
				numSamples++;
			}
		}
	}
	if (numSamples == 0)
		return 0.0f;
	return (height / numSamples) + 2.0f;
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