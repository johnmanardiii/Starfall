#version 330 core
out vec4 color;
in vec3 vertex_normal_n;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform sampler2D starTexture;


void main()
{
    vec3 lightPos = vec3(50,30,50);
    vec3 lightDir = normalize(lightPos - vertex_pos);
    float diffuse = dot(vertex_normal_n, lightDir);
    diffuse = clamp(diffuse, 0.2f, 1.0f);
    vec3 tcol = texture(starTexture, vertex_tex).rgb;


    color = vec4(tcol * diffuse,1);

}