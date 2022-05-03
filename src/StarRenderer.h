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

class StarRenderer : public Renderer
{
public:
    StarRenderer(string mod, string tex, string shader, string gameObject) : Renderer(gameObject)
    {
        ShaderManager shaderMan = ShaderManager::GetInstance();
        prog = shaderMan.GetShader(shader);
        texture = shaderMan.GetTexture(tex);
        model = shaderMan.GetModel(mod);
    }
    void Update(float frameTime, ComponentManager* compMan);
    void Init(ComponentManager* compMan);
    void Draw(float frameTime);

    shared_ptr<Shape> model;
    float totalTime = 0;
    GLuint texture;
};

