#version 410 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

// ACES tonemapping curve from: https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
vec3 ACESFilm(vec3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

void main()
{ 
    vec3 fb_color = texture(screenTexture, TexCoord).rgb;
    fb_color += texture(bloomTexture, TexCoord).rgb;
    // fb_color = ACESFilm(fb_color);
    // vec3 fb_color = texture(bloomTexture, TexCoord).rgb;
    color = vec4(fb_color, 1.0);
}