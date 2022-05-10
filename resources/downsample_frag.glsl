#version 410 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D threshTex;

// Better, temporally stable box filtering https://github.com/Unity-Technologies/Graphics/blob/master/com.unity.postprocessing/PostProcessing/Shaders/Sampling.hlsl
// [Jimenez14] http://goo.gl/eomGso
// . . . . . . .
// . A . B . C .
// . . D . E . .
// . F . G . H .
// . . I . J . .
// . K . L . M .
// . . . . . . .
vec4 DownsampleBox13Tap()
{
    vec2 texelSize = 1.0 / textureSize(threshTex, 0); // gets size of single texel
    vec4 A = texture(threshTex, TexCoord + texelSize * vec2(-1.0, -1.0));
    vec4 B = texture(threshTex, TexCoord + texelSize * vec2( 0.0, -1.0));
    vec4 C = texture(threshTex, TexCoord + texelSize * vec2( 1.0, -1.0));
    vec4 D = texture(threshTex, TexCoord + texelSize * vec2(-0.5, -0.5));
    vec4 E = texture(threshTex, TexCoord + texelSize * vec2( 0.5, -0.5));
    vec4 F = texture(threshTex, TexCoord + texelSize * vec2(-1.0,  0.0));
    vec4 G = texture(threshTex, TexCoord);
    vec4 H = texture(threshTex, TexCoord + texelSize * vec2( 1.0,  0.0));
    vec4 I = texture(threshTex, TexCoord + texelSize * vec2(-0.5,  0.5));
    vec4 J = texture(threshTex, TexCoord + texelSize * vec2( 0.5,  0.5));
    vec4 K = texture(threshTex, TexCoord + texelSize * vec2(-1.0,  1.0));
    vec4 L = texture(threshTex, TexCoord + texelSize * vec2( 0.0,  1.0));
    vec4 M = texture(threshTex, TexCoord + texelSize * vec2( 1.0,  1.0));

    vec2 div = (1.0 / 4.0) * vec2(0.5, 0.125);

    vec4 o = (D + E + I + J) * div.x;
    o += (A + B + G + F) * div.y;
    o += (B + C + H + G) * div.y;
    o += (F + G + L + K) * div.y;
    o += (G + H + M + L) * div.y;

    return o;
}

void main()
{ 
    // vec4 fb_color = DownsamplePS(TexCoord);
    // vec4 fb_color = DownsampleBox13Tap();
    color = DownsampleBox13Tap();
}