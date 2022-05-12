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
uniform vec3 camoff;

float heightCalc(float x, float z)
{
	//return z * 0.2f + sin(x / 20.0) * sin(z / 20.0) * 20.0 * cos(x / 20.) * cos(z / 15.);
	float dist = abs(x - 20.0f * sin(z / 100.0f) * cos(z / 50.0f));
	dist = max(30.0f, dist);
	dist -= 30.0f;
	return z * 0.6f + min(50.0f, dist);
}

// from user Appas www.stackoverflow.com/quesitons/4200224/random-noise-functions-for-glsl
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	vec2 texcoords=vertTex;
	float t=1./100.;
	texcoords += vec2(camoff.x,camoff.z)*t;
	float height = texture(tex, texcoords/100.).r;
	height *= 40.0;


	vec4 tpos =  vec4(vertPos, 1.0);
	tpos =  M * tpos;
	tpos.z +=camoff.z - 50.0f;
	tpos.x +=camoff.x;

	float proc_height = heightCalc(tpos.x, tpos.z);

	//proc_height = sin(tpos.x / 10.) * sin(tpos.z / 10.) * 10.0 * cos(tpos.x * 0.2) * cos(tpos.z * 0.1);
	tpos.y -= 5;

	vec3 b = tpos.xyz + vec3(0.1f, 0.0f, 0.1f), 
		c = tpos.xyz + vec3(0.1f, 0.0f, -0.1f);

	tpos.y += proc_height;
	b.y += heightCalc(b.x, b.z);
	c.y += heightCalc(c.x, c.z);
	/*b.y += sin(b.x / 10.) * sin(b.z / 10.) * 10.0 * cos(b.x * 0.2) * cos(b.z * 0.1);
	c.y += sin(c.x / 10.) * sin(c.z / 10.) * 10.0 * cos(c.x * 0.2) * cos(c.z * 0.1);*/
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
}
