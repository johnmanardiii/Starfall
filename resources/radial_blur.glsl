#version 410 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D screenTexture;

const int num_blur_samples = 20;    // this might be too much, when we get to performance I should check this out.
const float start_step_size = 0.0;
const float max_step_size = 10.0;

void main()
{
    vec2 outwardsVec = normalize(TexCoord - vec2(.5)) / textureSize(screenTexture, 0);  // pixel that samples one pixel outwards
    float step_size = mix(start_step_size, max_step_size,
        clamp((distance(vec2(.5), TexCoord) / .7071) - .2, 0, 1));   // increase blur step size by dist from center.
    outwardsVec *= step_size;
    // Get the initial color at this pixel.    
    vec4 blurred_color = texture(screenTexture, TexCoord);
    vec4 original_color = blurred_color;
    vec2 uv = TexCoord;
    uv += outwardsVec; 
    float num_added_samples = 1.0;
    for(int i = 1; i < num_blur_samples; ++i, uv += outwardsVec) 
    {   
        // Sample the color buffer along the velocity vector.  
        if(uv.x > 0 && uv.x < 1 && uv.y > 0 && uv.y < 1)
        {
            vec4 currentColor = texture(screenTexture, uv);   
            // Add the current color to our color sum.   
            blurred_color += currentColor; 
            // add one to the total divisor
            num_added_samples += 1.0f;
        }
    } // Average all of the samples to get the final blur color.    
    blurred_color = blurred_color / num_added_samples;
    color = blurred_color;
}