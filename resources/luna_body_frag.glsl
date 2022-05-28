#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform float flashAmt;
uniform sampler2D tex;
uniform sampler2D emissiveTex;
uniform vec3 flashCol;
void main()
{
	vec2 texture_uvs = vec2(vertex_tex.x, -vertex_tex.y);
	vec3 n = normalize(vertex_normal);
	vec3 lp=vec3(50, 30, 50);
	vec3 ld = normalize(lp - vertex_pos);
	float diffuse = dot(n,ld);
	vec3 tcol= texture(tex, texture_uvs).rgb;
	color.rgb = (1- flashAmt) * tcol + flashAmt * flashCol;
	diffuse = clamp(diffuse, 0.1f, 1.0f);
	color *= diffuse;
	// emissive map
	vec3 ecol= texture(emissiveTex, texture_uvs).rgb;
	color.rgb += ecol * 8;
	color.a = 1;
}