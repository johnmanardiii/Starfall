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
  vec3 lp=vec3(100, -30, 100);
  vec3 ld = normalize(lp - vertex_pos);
  float diffuse = dot(n,ld);
  vec3 tcol= texture(tex, vertex_tex).rgb;
  color.rgb = (1- flashAmt) * tcol + flashAmt * flashCol;
  color.a = 1;
  diffuse = clamp(abs(diffuse), 0.3f, 1.0f);
  color *= vec4(diffuse, diffuse, diffuse, 1);
}