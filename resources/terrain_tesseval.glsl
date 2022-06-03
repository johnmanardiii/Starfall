#version 410 core

layout (triangles, fractional_even_spacing, cw) in;	//equal_spacing
uniform mat4 P;
uniform mat4 V;
uniform sampler2D tex;
uniform vec3 camoff;
uniform float baseHeight;
out vec2 frag_tex;
out float frag_height;
out vec3 frag_pos;
out vec3 frag_norm;

float heightCalc(float x, float z)
{
	return z * 0.2f + sin(x / 20.0) * sin(z / 20.0) * 20.0 * cos(x / 20.) * cos(z / 15.);
}

void main()
{
	vec3 vertPos = (gl_TessCoord.x * gl_in[0].gl_Position +
                   gl_TessCoord.y * gl_in[1].gl_Position +
                   gl_TessCoord.z * gl_in[2].gl_Position).xyz;
	vertPos.y = baseHeight;
	vec4 tpos =  vec4(vertPos, 1.0);

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

	vec2 texcoords = vec2(tpos.x / 100., tpos.z / 100.);

	gl_Position = P * V * tpos;
	frag_pos = tpos.xyz;
	frag_tex = texcoords;
	frag_norm = n;
	frag_height = tpos.y;
}