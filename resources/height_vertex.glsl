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
out vec3 frag_col;
uniform sampler2D tex;
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
	vec2 texcoords=vertTex;
	float t=1/100.;
	texcoords += vec2(-camoff.x, camoff.z)*t;
	float height = texture(tex, texcoords/40.).r;
	height *= 300.0;


	vec4 tpos =  vec4(vertPos, 1.0);
	tpos =  M * tpos;
	tpos.z += camoff.z;
	tpos.x += camoff.x;

	vec3 b = tpos.xyz + vec3(0.1f, 0.0f, 0.1f), 
		c = tpos.xyz + vec3(0.1f, 0.0f, -0.1f);
	vec2 bUV = texcoords + vec2(0.01f / 40., 0.01f / 40.),
		cUV = texcoords + vec2(0.01f / 40., -0.01f / 40.);
	tpos.y += height;
	b.y += texture(tex, bUV/40.).r * 300.0f;
	c.y += texture(tex, cUV/40.).r * 300.0f;
	vec3 ab = (tpos.xyz - b);
	ab = normalize(ab);
	vec3 ac = (tpos.xyz - c);
	ac = normalize(ac);
	vec3 n = normalize(cross(ab,ac));
	gl_Position = P * V * tpos;
	frag_pos = tpos.xyz;
	frag_height = tpos.y;
	frag_tex = vertTex;
	frag_norm = n;
	frag_col = camoff;
}
