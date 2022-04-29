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

    vec3 distFromCenterToEdge = tpos.xyz - centerPos;

    gl_Position = M * vec4(vertPos - 2 * (distFromCenterToEdge), 1.0f);

    gl_Position = P * V * gl_Position;
	vertex_tex = vertTex * vec2(1, -1);
}