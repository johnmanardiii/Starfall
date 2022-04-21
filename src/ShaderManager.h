#pragma once
#include "Program.h"
#include "Shape.h"
#include <map>
#include <string>
#include <memory>

struct TerrainS {
	GLuint VAOId;
	GLuint IndexBuff;
	int numVerts;
};

using namespace std;
class ShaderManager
{
public:
	shared_ptr<Program> GetShader(string);
	GLuint GetTexture(string name) { return textures[name]; }
	shared_ptr<Shape> GetModel(string);
	TerrainS Terrain;

	void SetShader(string, shared_ptr<Program>);
	void SetTexture(string name, GLuint tex) { textures[name] = tex; }
	void SetModel(string, shared_ptr<Shape>);

	static ShaderManager& GetInstance() 
	{
		static ShaderManager instance;
		return instance;
	}
private:
	map<string, shared_ptr<Program>> shaders;
	map<string, GLuint> textures;
	map<string, shared_ptr<Shape>> models;

	ShaderManager() {}
};