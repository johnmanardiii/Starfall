#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform sampler2D tex;
void main()
{
	vec3 n = normalize(vertex_normal);
  vec3 lp=vec3(100, -30, 100);
  vec3 ld = normalize(lp - vertex_pos);
  float diffuse = dot(n,ld);
  vec3 tcol= texture(tex, vertex_tex).rgb;
  color = vec4(tcol, 1.0f);
  diffuse = clamp(abs(diffuse), 0.3f, 1.0f);
  color *= vec4(diffuse, diffuse, diffuse, 1);
}