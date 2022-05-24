#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform float flashAmt;
uniform sampler2D tex;
uniform vec3 flashCol;

vec3 draw_eyes(vec3 base_color)
{
	float radius = .1;
	vec2 center = vec2(.2, .2);
	vec3 eye_color = vec3(.28, .88, 1) * 1.2;
	// TODO: generalize this and remove if statement
	if(distance(center, vertex_tex) < radius)
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
	vec3 tcol= texture(tex, vertex_tex).rgb;
	color.rgb = (1- flashAmt) * tcol + flashAmt * flashCol;
	diffuse = clamp(diffuse, 0.1f, 1.0f);
	color *= diffuse;
	// color *= 1000;
	
	//color.rgb = draw_eyes(color.rgb);
	// color.rgb = vec3(vertex_tex / 2, 0);
	if(vertex_tex.y > .5)
	{
		color.rgb = vec3(1);
	}
	else
	{
		color.rgb = vec3(0);
	}
	// color.rgb = vec3(vertex_tex.y);
	color.a = 1;
}