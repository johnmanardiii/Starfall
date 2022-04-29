#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float totalTime;
uniform float centerPos;

out vec3 vertex_pos;
out vec3 vertex_normal_n;
out vec2 vertex_tex;

void main()
{
	vertex_normal_n = normalize(vec4(M * vec4(vertNor,0.0)).xyz);
	vec4 tpos =  M * vec4(vertPos, 1.0);
	vertex_pos = tpos.xyz;

    vec3 distFromCenterToEdge = vertex_pos - centerPos;

    gl_Position = P * V * M * vec4(vertPos, 1.0f);
	vertex_tex = vertTex * vec2(1, -1);
}