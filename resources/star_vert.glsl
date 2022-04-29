#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float totalTime;

out vec3 vertex_pos;
out vec3 vertex_normal_n;
out vec2 vertex_tex;

void main()
{
	vertex_normal_n = normalize(vec4(M * vec4(vertNor,0.0)).xyz);
	vec4 tpos =  M * vec4(vertPos, 1.0);
	vertex_pos = tpos.xyz;
	gl_Position = P * V * tpos;

    //get the distance a particular point is from the center
    vec4 modelTranslation = vec4(M[0][3], M[1][3], M[2][3], 1.0f);

    vec3 centerPosition = (P * V * modelTranslation).xyz;
    vec3 distFromCenterToEdge = gl_Position.xyz - centerPosition;

    gl_Position = vec4(0.5f * sin(totalTime) * normalize(distFromCenterToEdge), 1.0f);

	vertex_tex = vertTex * vec2(1, -1);
}