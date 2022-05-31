#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;
in vec3 lTS;


uniform float flashAmt;
uniform sampler2D tex;
uniform sampler2D emissiveTex;
uniform sampler2D normalTex;
uniform vec3 flashCol;


void main()
{
	vec2 texture_uvs = vec2(vertex_tex.x, -vertex_tex.y);
	vec3 n = normalize(vertex_normal);
	
	vec3 tcol= texture(tex, texture_uvs).rgb;
	color.rgb = (1- flashAmt) * tcol + flashAmt * flashCol;

	vec3 normal = texture(normalTex, texture_uvs).xyz;
	normal = (normal * 2) - vec3(1.0);
	float dCoeff = max(0.0, dot(lTS, normal));
	color *= dCoeff;
	// emissive map
	vec3 ecol= texture(emissiveTex, texture_uvs).rgb;
	color.rgb += ecol * 8;
	// color.rgb = normal;
	color.a = 1;
}