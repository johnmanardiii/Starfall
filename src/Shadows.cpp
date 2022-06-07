#include "Shadows.h"

void Shadows::InitializeShadowFBO()
{
    //generate the FBO for the shadow depth
    glGenFramebuffers(1, &depthMapFBO);

    //generate the texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, S_WIDTH, S_HEIGHT,
        0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //bind with framebuffer's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadows::RenderShadowPass()
{
    Camera cam = Camera::GetInstance(vec3(0, 1, 0));
    mat4 P = cam.GetPerspective();

    //set up light's depth map
    glViewport(0, 0, S_WIDTH, S_HEIGHT);

    //sets up the output to be out FBO
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    //set up shadow shader and render the scene
    prog->bind();
    //TODO you will need to fix these
    /*
    LO = SetOrthoMatrix(DepthProg);
    LV = SetLightView(DepthProg, g_light, lightLA, lightUp);
    drawScene(DepthProg, 0, 0);
    DepthProg->unbind();*/

    //set culling back to normal
    glCullFace(GL_BACK);

    //this sets the output back to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
void SetProjectionMatrix(shared_ptr<Program> curShade) {
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    float aspect = width / (float)height;
    mat4 Projection = perspective(radians(50.0f), aspect, 0.1f, 100.0f);
    glUniformMatrix4fv(curShade->getUniform("P"), 1, GL_FALSE, value_ptr(Projection));
}

mat4 SetOrthoMatrix(shared_ptr<Program> curShade) {
    mat4 ortho = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 100.0f);

    glUniformMatrix4fv(curShade->getUniform("LP"), 1, GL_FALSE, value_ptr(ortho));
    return ortho;
}

mat4 SetLightView(shared_ptr<Program> curShade, vec3 pos, vec3 LA, vec3 up) {
    //mat4 Cam = mat4(1.0); 
    mat4 Cam = glm::lookAt(pos, LA, up);
    glUniformMatrix4fv(curShade->getUniform("LV"), 1, GL_FALSE, value_ptr(Cam));
    return Cam;
}*/