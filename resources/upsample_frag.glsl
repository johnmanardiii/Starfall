#version 410 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D lowRes;
uniform sampler2D currentRes;

// 9-tap bilinear upsampler (tent filter) https://github.com/Unity-Technologies/Graphics/blob/master/com.unity.postprocessing/PostProcessing/Shaders/Sampling.hlsl
vec4 UpsampleTent()
{
    float sampleScale = 1;
    vec2 texelSize = 1.0 / textureSize(lowRes, 0);
    vec4 d = texelSize.xyxy * vec4(1.0, 1.0, -1.0, 0.0) * sampleScale;

    vec4 s;
    s =  texture(lowRes, TexCoord - d.xy);
    s += texture(lowRes, TexCoord - d.wy) * 2.0;
    s += texture(lowRes, TexCoord - d.zy);

    s += texture(lowRes, TexCoord + d.zw) * 2.0;
    s += texture(lowRes, TexCoord       ) * 4.0;
    s += texture(lowRes, TexCoord + d.xw) * 2.0;

    s += texture(lowRes, TexCoord + d.zy);
    s += texture(lowRes, TexCoord + d.wy) * 2.0;
    s += texture(lowRes, TexCoord + d.xy);

    return s * (1.0 / 16.0);
}

void main()
{ 
    vec4 upsample_color = UpsampleTent();
    vec4 currentRes_color = texture(currentRes, TexCoord);

    // QUESTION
    // TO PROFESSOR: The upsample, as used here, does not fade
    // away the color nearly enough on its own. I implemented my own
    // exponential falloff of the colors, but it doesn't seem to
    // be the right way of doing this and is really touchy.

    // additionally, the brightness is unfortunately dependent on the number
    // of upsamples as well because of this.
    color = upsample_color * .34 + currentRes_color;
}