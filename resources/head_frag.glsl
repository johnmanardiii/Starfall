#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform float flashAmt;
uniform sampler2D tex;
uniform vec3 flashCol;
uniform vec2 eye1Pos;
uniform float eye1Radius;
uniform float eyeOpenPct;

bool isUVInEye(vec2 eyePos, vec2 uv)
{
	float yRadius = 3.0 * eye1Radius;	// eclipse scale
	yRadius *= eyeOpenPct;
	float xdist = ((uv.x - eyePos.x) * (uv.x - eyePos.x)) / (eye1Radius * eye1Radius);
	float ydist = ((uv.y - eyePos.y) * (uv.y - eyePos.y)) / (yRadius * yRadius);
	return xdist + ydist <= 1;
}

vec3 draw_eyes(vec3 base_color)
{
	float radius = eye1Radius;
	vec2 eye2Pos = vec2(1 - eye1Pos.x, eye1Pos.y);
	vec3 eye_color = vec3(.28, .88, 1) * 1.4;
	// TODO: generalize this and remove if statement
	if(isUVInEye(eye1Pos, vertex_tex) || isUVInEye(eye2Pos, vertex_tex))
	{
		return eye_color;
	}
	else
	{
		return base_color;
	}
}

void main()
{
	vec3 n = normalize(vertex_normal);
	vec3 lp=vec3(50, 30, 50);
	vec3 ld = normalize(lp - vertex_pos);
	float diffuse = dot(n,ld);
	// vec3 tcol= texture(tex, texture_uvs).rgb;
	vec3 tcol = vec3(1, 1, 1);	// this is the case since the head has weird UVs and needs its own individual tex.
	color.rgb = (1- flashAmt) * tcol + flashAmt * flashCol;
	diffuse = clamp(diffuse, 0.1f, 1.0f);
	color *= diffuse;
	
	color.rgb = draw_eyes(color.rgb);
	// color.rgb = vec3(vertex_tex / 2, 0);
	// color.rgb = vec3(vertex_tex, 0);
	color.a = 1;
}