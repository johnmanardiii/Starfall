#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
layout(location = 3) in vec3 vertTan;
layout(location = 4) in vec3 vertBN;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec3 vertex_pos;
out vec3 vertex_normal;
out vec2 vertex_tex;
out vec3 lTS;


void main()
{
	vertex_normal = vec4(M * vec4(vertNor,0.0)).xyz;
	vec4 tpos =  M * vec4(vertPos, 1.0);
	vertex_pos = tpos.xyz;
	gl_Position = P * V * tpos;
	vertex_tex = vertTex * vec2(1, 1);

	mat3 TBN;
	vec3 wN = vertex_normal;
	vec3 wT = (M*vec4(vertTan, 0.0)).xyz;
	vec3 wBN = (M*vec4(vertBN, 0.0)).xyz;
	TBN = transpose(mat3(wT, wBN, wN));
	vec3 lp=vec3(50, 30, 50);

	lTS = TBN * normalize(lp - vertex_pos);
}