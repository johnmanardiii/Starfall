#pragma once
class ComponentManager;
#include "Renderer.h"
#include "Shape.h"
#include "ShaderManager.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //for lookat
#include <glm/gtc/type_ptr.hpp> //for value_ptr

#include "Camera.h"

class TextureRenderer : public Renderer
{
public:
	TextureRenderer(string mod, string tex, string gameObject) : Renderer(gameObject)
	{
		ShaderManager shaderMan = ShaderManager::GetInstance();
		prog = shaderMan.GetShader("Texture");
		texture = shaderMan.GetTexture(tex);
		model = shaderMan.GetModel(mod);
	}
	void Update(float frameTime) { Draw(frameTime); }
	void Init(ComponentManager& compMan);

	void Draw(float frameTime);
	void SetFlashAmt(float amt) { flashAmt = amt; }
private:
	shared_ptr<Shape> model;
	GLuint texture;
	float flashAmt = 0;
	vec3 flashColor = vec3(1, 0, 1);
};