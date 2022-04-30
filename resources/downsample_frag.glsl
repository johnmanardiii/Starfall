#version 410 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D threshTex;

// A lot of code converted from: https://github.com/Kosmonaut3d/BloomFilter-for-Monogame-and-XNA/blob/master/Bloom%20Sample/Content/Shaders/BloomFilter/Bloom.fx
// since the method of downsampling was a bit complicated (need to ask if this is ok in office hours)
vec4 Box4(vec4 p0, vec4 p1, vec4 p2, vec4 p3)
{
	return (p0 + p1 + p2 + p3) * 0.25f;
}

//Downsample to the next mip, blur in the process
vec4 DownsamplePS(vec2 texCoord)
{
    vec2 InverseResolution = 1.0 / textureSize(threshTex, 0); // gets size of single texel
    vec2 offset = vec2(InverseResolution.x, InverseResolution.y);

	vec2 halfPixel = InverseResolution / 2;
        
    vec4 c0 = texture(threshTex, texCoord + vec2(-2, -2) * offset + halfPixel);
    vec4 c1 = texture(threshTex, texCoord + vec2(0,-2)*offset + halfPixel);
    vec4 c2 = texture(threshTex, texCoord + vec2(2, -2) * offset + halfPixel);
    vec4 c3 = texture(threshTex, texCoord + vec2(-1, -1) * offset + halfPixel);
    vec4 c4 = texture(threshTex, texCoord + vec2(1, -1) * offset + halfPixel);
    vec4 c5 = texture(threshTex, texCoord + vec2(-2, 0) * offset + halfPixel);
    vec4 c6 = texture(threshTex, texCoord + halfPixel);
    vec4 c7 = texture(threshTex, texCoord + vec2(2, 0) * offset + halfPixel);
    vec4 c8 = texture(threshTex, texCoord + vec2(-1, 1) * offset + halfPixel);
    vec4 c9 = texture(threshTex, texCoord + vec2(1, 1) * offset + halfPixel);
    vec4 c10 = texture(threshTex, texCoord + vec2(-2, 2) * offset + halfPixel);
    vec4 c11 = texture(threshTex, texCoord + vec2(0, 2) * offset + halfPixel);
    vec4 c12 = texture(threshTex, texCoord + vec2(2, 2) * offset + halfPixel);

    return Box4(c0, c1, c5, c6) * 0.125f +
    Box4(c1, c2, c6, c7) * 0.125f +
    Box4(c5, c6, c10, c11) * 0.125f +
    Box4(c6, c7, c11, c12) * 0.125f +
    Box4(c3, c4, c8, c9) * 0.5f;
}

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