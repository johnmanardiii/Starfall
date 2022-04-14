#include "ShaderManager.h"

shared_ptr<Program> ShaderManager::GetShader(string name)
{
	shared_ptr<Program> shaderPtrCopy = shaders[name];
	return shaderPtrCopy;
}

shared_ptr<Shape> ShaderManager::GetModel(string name)
{
	shared_ptr<Shape> modelPtrCopy = models[name];
	return modelPtrCopy;
}

void ShaderManager::SetShader(string name, shared_ptr<Program> shader)
{
	shared_ptr<Program> shaderPtrCopy = shader;
	shaders[name] = shader;
}

void ShaderManager::SetModel(string name, shared_ptr<Shape> model)
{
	shared_ptr<Shape> modelPtrCopy = model;
	models[name] = modelPtrCopy;
}