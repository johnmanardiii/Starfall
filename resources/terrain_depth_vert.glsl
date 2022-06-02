#version 410 core
layout(location = 0) in vec3 vertPos;

uniform mat4 M;
uniform mat4 LP;
uniform mat4 LV;

uniform vec3 camoff;

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
	tpos.z +=camoff.z;
	tpos.x +=camoff.x;

	float proc_height = heightCalc(tpos.x, tpos.z);

	tpos.y -= 5;

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

	gl_Position = LP * LV * tpos;
}
