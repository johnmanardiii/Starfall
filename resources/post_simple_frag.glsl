#version 410 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

// A lot of code converted from: https://github.com/Kosmonaut3d/BloomFilter-for-Monogame-and-XNA/blob/master/Bloom%20Sample/Content/Shaders/BloomFilter/Bloom.fx
// since the method of downsampling was a bit complicated (need to ask if this is ok in office hours)

//Upsample to the former MIP, blur in the process
vec4 UpsamplePS(vec2 texCoord)
{
    vec2 InverseResolution = 1.0 / textureSize(bloomTexture, 0); // gets size of single texel
    vec2 offset = vec2(InverseResolution.x, 1 * InverseResolution.y); // * Radius;
    // vec2 offset = vec2(InverseResolution.x, 1 * InverseResolution.y) * 5.0;

	vec2 halfPixel = InverseResolution / 2;

    vec4 c0 = texture(bloomTexture, texCoord + vec2(-1, -1) * offset + halfPixel);
    vec4 c1 = texture(bloomTexture, texCoord + vec2(0, -1) * offset + halfPixel);
    vec4 c2 = texture(bloomTexture, texCoord + vec2(1, -1) * offset + halfPixel);
    vec4 c3 = texture(bloomTexture, texCoord + vec2(-1, 0) * offset + halfPixel);
    vec4 c4 = texture(bloomTexture, texCoord + halfPixel);
    vec4 c5 = texture(bloomTexture, texCoord + vec2(1, 0) * offset + halfPixel);
    vec4 c6 = texture(bloomTexture, texCoord + vec2(-1,1) * offset + halfPixel);
    vec4 c7 = texture(bloomTexture, texCoord + vec2(0, 1) * offset + halfPixel);
    vec4 c8 = texture(bloomTexture, texCoord + vec2(1, 1) * offset + halfPixel);

    //Tentfilter  0.0625f    
    return 0.0625f * (c0 + 2 * c1 + c2 + 2 * c3 + 4 * c4 + 2 * c5 + c6 + 2 * c7 + c8); // * Strength + vec4(0, 0,0,0); //TODO: add uniform strength and radius??

}

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
    // vec3 fb_color = vec3(0);
    //fb_color += UpsamplePS(TexCoord).rgb;
    vec3 bloom_color = texture(bloomTexture, TexCoord).rgb;
    fb_color += bloom_color.rgb * .05;
    fb_color = ACESFilm(fb_color);
    color = vec4(fb_color, 1.0);
}