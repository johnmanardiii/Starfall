#include "HUDRenderer.h"
#include "Camera.h"
#include "LightComponent.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

void HUDRenderer::Init(GameState* gameState)
{
    ShaderManager shaderMan = ShaderManager::GetInstance();

    InitQuad();
    int totalStarsRequired = gameState->GetObjectsNeeded();
    int tenthPlace = (totalStarsRequired / 10) % 10;
    int onesPlace = (totalStarsRequired % 10);
    starsRequired_tenthPlace_tex = shaderMan.GetTexture("num_" + to_string(tenthPlace));
    starsRequired_onesPlace_tex = shaderMan.GetTexture("num_" + to_string(onesPlace));
    win_tex = shaderMan.GetTexture("win");
    lose_tex = shaderMan.GetTexture("lose");
    slash_tex = shaderMan.GetTexture("slash");
    x_tex = shaderMan.GetTexture("x");
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

void HUDRenderer::Update(float frameTime, GameState* gameState)
{
    ShaderManager shaderMan = ShaderManager::GetInstance();

    moonIconPosition = glm::mix(moonIconEndingPos, moonIconStartingPos, gameState->timeLeft / gameState->startTime);
    int totalStarsCollected = gameState->ReportObjectsCollected();
    int tenthPlace = (totalStarsCollected / 10) % 10;
    int onesPlace = (totalStarsCollected % 10);
    if (tenthPlace == 0)
        starsCollected_tenthPlace_tex = shaderMan.GetTexture("num_10");
    else
        starsCollected_tenthPlace_tex = shaderMan.GetTexture("num_" + to_string(tenthPlace));
    starsCollected_onesPlace_tex = shaderMan.GetTexture("num_" + to_string(onesPlace));
}

void HUDRenderer::SetUpFrameBuffers(int width, int height)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    projection = glm::ortho(0.0f, width * 1.0f, height * 1.0f, 0.0f, -1.0f, 1.0f);
}

void HUDRenderer::RenderHUD(int width, int height, GameState* gameState)
{
    SetUpFrameBuffers(width, height);

	prog->bind();

    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &projection[0][0]);
    DrawSprite(moonBar_tex, vec2(50.0f, 50.0f), vec2(700.0f, 50.0f), 0.0f);
    DrawSprite(moonIcon_tex, vec2(moonIconPosition, 35.0f), vec2(80.0f, 80.0f), 0.0f);
    DrawSprite(starFragment_tex, vec2(20.0f, 105.0f), vec2(150.0f, 150.0f), 0.0f);

    DrawSprite(x_tex, vec2(135, 145.0f), vec2(50.0f, 50.0f), 0.0f);

    DrawSprite(starsCollected_tenthPlace_tex, vec2(170, 138), vec2(70, 70), 0.0f);
    DrawSprite(starsCollected_onesPlace_tex, vec2(210, 138), vec2(70, 70), 0.0f);

    DrawSprite(slash_tex, vec2(245, 134), vec2(80, 80), 0.0f);
    DrawSprite(starsRequired_tenthPlace_tex, vec2(290, 138), vec2(70, 70), 0.0f);
    DrawSprite(starsRequired_onesPlace_tex, vec2(330, 138), vec2(70, 70), 0.0f);

    if (gameState->IsGameEnded())
    {
        if (gameState->wonGame)
            DrawSprite(win_tex, vec2(width / 2 - 300, height - 400), vec2(600.0f, 230.0f), 0.0f);
        else
            DrawSprite(lose_tex, vec2(width / 2 - 300, height - 400), vec2(600.0f, 230.0f), 0.0f);
    }

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