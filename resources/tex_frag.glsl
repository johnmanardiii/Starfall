#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform float flashAmt;
uniform sampler2D tex;
uniform vec3 flashCol;
void main()
{
	vec3 n = normalize(vertex_normal);
  vec3 lp=vec3(50, 30, 50);
  vec3 ld = normalize(lp - vertex_pos);
  float diffuse = dot(n,ld);
  vec3 tcol= texture(tex, vertex_tex).rgb;
  color.rgb = (1- flashAmt) * tcol + flashAmt * flashCol;
  diffuse = clamp(diffuse, 0.2f, 1.0f);
  color *= diffuse;
  color.a = 1;
}