#version 330 core
out vec4 color;
in vec3 vertex_pos;
in vec2 vertex_tex;
in float vertex_height;

uniform sampler2D tex;
uniform sampler2D tex2;
uniform vec3 camoff;
uniform vec3 campos;

void main()
{
vec2 texcoords=vertex_tex;
float t=1./100.;
texcoords -= vec2(camoff.x,camoff.z)*t;

color.rgb = texture(tex2, texcoords*50).rgb * ((vertex_height + 10) / 20);
color.a=1;

float len = length(vertex_pos.xz+campos.xz);
len-=41;
len/=8.;
len=clamp(len,0,1);
color.a=1-len;


}
