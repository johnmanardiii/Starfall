#version 410 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec3 vertex_pos;
out vec2 vertex_tex;
out float vertex_height;
uniform sampler2D tex;
uniform vec3 camoff;

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
	tpos.z +=camoff.z;
	tpos.x +=camoff.x;

	float proc_height = rand(floor(tpos.xz * 0.05)) * 0;
	proc_height += rand(floor(tpos.xz * 0.1)) * 0.8;
	proc_height += rand(floor(tpos.xz * 0.5)) * 0.4;
	proc_height += rand(floor(tpos.xz)) * 0.3;

	proc_height = sin(tpos.x / 10.) * sin(tpos.z / 10.) * 10 * cos(tpos.x * 0.2) * cos(tpos.z * 0.1);
	//proc_height = sin(tpos.x / 10.) * 5 * sin(tpos.x * 0.07);
	tpos.y -= 5;
	//tpos.y += height;
	tpos.y += proc_height;
	vertex_pos = tpos.xyz;
	vertex_height = tpos.y;

	gl_Position = tpos;
	vertex_tex = vertTex;
}
