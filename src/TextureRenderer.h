#pragma once

#include "Renderer.h"
#include "Shape.h"
#include "ShaderManager.h"
#include <memory>

class TextureRenderer : Renderer
{
public:
	TextureRenderer(string mod, string tex, string gameObject) : Renderer(gameObject)
	{
		ShaderManager shaderMan = ShaderManager::GetInstance();
		prog = shaderMan.GetShader("Texture");
		texture = shaderMan.GetTexture(tex);
		model = shaderMan.GetModel(mod);
	}
	void Update() { Draw(); }
	void Init(ComponentManager&);

	void Draw();
private:
	shared_ptr<Shape> model;
	GLuint texture;

};