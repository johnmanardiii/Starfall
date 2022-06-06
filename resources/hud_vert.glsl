#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec2 vertTex;

out vec2 texCoord;
uniform mat4 M;
uniform mat4 P;

void main()
{
	//gl_Position = P * M * vec4(vertPos.xy, 0.0, 1);
	gl_Position = P * M * vec4(vertPos.xy, 0, 1);

	texCoord = vertPos.zw;
}
