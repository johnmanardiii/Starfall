#version 410 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec3 frag_pos;
out vec2 frag_tex;
out vec3 frag_norm;
out float frag_height;
uniform sampler2D tex;

float heightCalc(float x, float z)
{
	return z * 0.2f + sin(x / 20.0) * sin(z / 20.0) * 20.0 * cos(x / 20.) * cos(z / 15.);
	// return z * .5;
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
	b.y += heightCalc(b.x, b.z);
	c.y += heightCalc(c.x, c.z);
	vec3 ab = (tpos.xyz - b);
	ab = normalize(ab);
	vec3 ac = (tpos.xyz - c);
	ac = normalize(ac);
	vec3 n = normalize(cross(ab,ac));

	vec2 texcoords=vertTex;
	texcoords = vec2(tpos.x / 100., tpos.z / 100.);

	gl_Position = P * V * tpos;
	frag_pos = tpos.xyz;
	frag_height = tpos.y;
	frag_tex = texcoords;
	frag_norm = n;
}
