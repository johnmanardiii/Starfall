#version 330 core
// code started from LearnOpenGL code
layout (location = 0) in vec2 vertPos;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(vertPos.x, vertPos.y, 0.0, 1.0); 
    TexCoord = texCoord;
}  