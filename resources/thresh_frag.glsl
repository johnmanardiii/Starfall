#version 410 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D screenTexture;

// luminance function from: https://github.com/CesiumGS/cesium/blob/master/Source/Shaders/Builtin/Functions/luminance.glsl
float luminance(vec3 rgb)
{
    // Algorithm from Chapter 10 of Graphics Shaders.
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    return dot(rgb, W);
}

void main()
{ 
    vec3 fb_color = texture(screenTexture, TexCoord, 0).rgb;
    if(luminance(fb_color) < 1.0)
    {
        color = vec4(0, 0, 0, 1);
    }
    else
    {
        color = vec4(fb_color, 1);
    }
    // color = vec4(1, 0, 0, 1);
}