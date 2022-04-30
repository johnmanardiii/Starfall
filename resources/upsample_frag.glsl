#version 410 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D lowRes;
uniform sampler2D currentRes;

// A lot of code converted from: https://github.com/Kosmonaut3d/BloomFilter-for-Monogame-and-XNA/blob/master/Bloom%20Sample/Content/Shaders/BloomFilter/Bloom.fx
// since the method of downsampling was a bit complicated (need to ask if this is ok in office hours)

//Upsample to the former MIP, blur in the process
vec4 UpsamplePS(vec2 texCoord)
{
    vec2 InverseResolution = 1.0 / textureSize(lowRes, 0); // gets size of single texel
    vec2 offset = vec2(InverseResolution.x, 1 * InverseResolution.y); // * Radius;
    // vec2 offset = vec2(InverseResolution.x, 1 * InverseResolution.y) * 5.0;

	vec2 halfPixel = InverseResolution / 2;

    vec4 c0 = texture(lowRes, texCoord + vec2(-1, -1) * offset + halfPixel);
    vec4 c1 = texture(lowRes, texCoord + vec2(0, -1) * offset + halfPixel);
    vec4 c2 = texture(lowRes, texCoord + vec2(1, -1) * offset + halfPixel);
    vec4 c3 = texture(lowRes, texCoord + vec2(-1, 0) * offset + halfPixel);
    vec4 c4 = texture(lowRes, texCoord + halfPixel);
    vec4 c5 = texture(lowRes, texCoord + vec2(1, 0) * offset + halfPixel);
    vec4 c6 = texture(lowRes, texCoord + vec2(-1,1) * offset + halfPixel);
    vec4 c7 = texture(lowRes, texCoord + vec2(0, 1) * offset + halfPixel);
    vec4 c8 = texture(lowRes, texCoord + vec2(1, 1) * offset + halfPixel);

    //Tentfilter  0.0625f    
    return 0.0625f * (c0 + 2 * c1 + c2 + 2 * c3 + 4 * c4 + 2 * c5 + c6 + 2 * c7 + c8); // * Strength + vec4(0, 0,0,0); //TODO: add uniform strength and radius??

}

//Upsample to the former MIP, blur in the process, change offset depending on luminance
vec4 UpsampleLuminancePS(vec2 texCoord)
{
    vec2 InverseResolution = 1.0 / textureSize(lowRes, 0); // gets size of single texel
	vec2 halfPixel = InverseResolution / 2;

	vec4 c4 = texture(lowRes, texCoord + halfPixel); //middle one
 
    /*float luminance = c4.r * 0.21f + c4.g * 0.72f + c4.b * 0.07f;
    luminance = max(luminance, 0.4f);
*/
	vec2 offset = vec2(InverseResolution.x, 1 * InverseResolution.y); // * Radius; /// luminance;

	vec4 c0 = texture(lowRes, texCoord + vec2(-1, -1) * offset + halfPixel);
	vec4 c1 = texture(lowRes, texCoord + vec2(0, -1) * offset + halfPixel);
	vec4 c2 = texture(lowRes, texCoord + vec2(1, -1) * offset + halfPixel);
	vec4 c3 = texture(lowRes, texCoord + vec2(-1, 0) * offset + halfPixel);
	vec4 c5 = texture(lowRes, texCoord + vec2(1, 0) * offset + halfPixel);
	vec4 c6 = texture(lowRes, texCoord + vec2(-1, 1) * offset + halfPixel);
	vec4 c7 = texture(lowRes, texCoord + vec2(0, 1) * offset + halfPixel);
	vec4 c8 = texture(lowRes, texCoord + vec2(1, 1) * offset + halfPixel);
 
    return 0.0625f * (c0 + 2 * c1 + c2 + 2 * c3 + 4 * c4 + 2 * c5 + c6 + 2 * c7 + c8); // * Strength + vec4(0, 0, 0, 0); //+ 0.5f * ScreenTexture.Sample(c_texture, texCoord);

}

void main()
{ 
    vec4 upsample_color = UpsamplePS(TexCoord);
    //vec4 upsample_color = texture(lowRes, TexCoord);
    vec4 currentRes_color = texture(currentRes, TexCoord);
    color = upsample_color + currentRes_color;
}