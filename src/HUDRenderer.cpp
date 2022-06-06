#include "HUDRenderer.h"
#include "Camera.h"
#include "LightComponent.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

void HUDRenderer::Init()
{
    InitQuad();
}

void HUDRenderer::InitQuad() {

    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void HUDRenderer::SetUpFrameBuffers(int width, int height)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    projection = glm::ortho(0.0f, width * 1.0f, height * 1.0f, 0.0f, -1.0f, 1.0f);
}

void HUDRenderer::RenderHUD(int width, int height)
{
    SetUpFrameBuffers(width, height);

	prog->bind();

    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &projection[0][0]);
    DrawSprite(starFragment_tex,vec2(30.0f, 200.0f), vec2(100.0f, 100.0f), 0.0f);
    DrawSprite(moonBar_tex, vec2(30.0f, 30.0f), vec2(700.0f, 100.0f), 0.0f);
    DrawSprite(moonIcon_tex, vec2(30.0f, 30.0f), vec2(100.0f, 100.0f), 0.0f);

	prog->unbind();
}

void HUDRenderer::DrawSprite(GLuint tex, vec2 position, vec2 size, float rotate)
{
    mat4 model = mat4(1.0f);
    model = translate(model, vec3(position, 0.0f));

    model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, radians(rotate), vec3(0.0f, 0.0f, 1.0f));
    model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &model[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(prog->getUniform("texBuf"), 0);

    //draw the quad
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}