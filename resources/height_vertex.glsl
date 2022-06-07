#version 410 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec3 vertex_pos;
uniform mat4 LS; // Composite L0 and LS 
out vec4 posLS;
uniform sampler2D tex;

float heightCalc(float x, float z)
{
	return z * 0.2f + sin(x / 20.0) * sin(z / 20.0) * 20.0 * cos(x / 20.) * cos(z / 15.);
}

// from user Appas www.stackoverflow.com/quesitons/4200224/random-noise-functions-for-glsl
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	vec4 tpos =  vec4(vertPos, 1.0);
	tpos =  M * tpos;

	float proc_height = heightCalc(tpos.x, tpos.z);
	vec3 b = tpos.xyz + vec3(0.1f, 0.0f, 0.1f), 
		c = tpos.xyz + vec3(0.1f, 0.0f, -0.1f);

	tpos.y += proc_height;

	gl_Position = tpos;
	vertex_pos = tpos.xyz;
	posLS = LS * tpos;
}
