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


// https://64.github.io/tonemapping/
vec3 uncharted2_tonemap_partial(vec3 x)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;
    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 uncharted2_filmic(vec3 v)
{
    float exposure_bias = 2.0f;
    vec3 curr = uncharted2_tonemap_partial(v * exposure_bias);

    vec3 W = vec3(11.2f);
    vec3 white_scale = vec3(1.0f) / uncharted2_tonemap_partial(W);
    return curr * white_scale;
}

// https://twitter.com/jimhejl/status/633777619998130176
vec3 ToneMapFilmicHejl(vec3 hdr, float whitePt)
{
    vec4 vh = vec4(hdr.x, hdr.y, hdr.z, whitePt);
    vec4 va = (1.425 * vh) + .5;
    vec4 vf = ((vh * va + .004) / ((vh * (va + .55) + .0491))) - .0021;
    return vf.rgb / vf.www;
}

void main()
{
    vec3 fb_color = texture(screenTexture, TexCoord).rgb;
    vec3 bloom_color = texture(bloomTexture, TexCoord).rgb;
    fb_color += bloom_color.rgb;
    fb_color = ACESFilm(fb_color);
    color = vec4(fb_color, 1.0);
}

